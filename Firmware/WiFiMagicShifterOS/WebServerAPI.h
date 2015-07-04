struct ServerConfig
{
  char hostname[48];
  int port;
};

struct HWInfo
{
  int leds;
  char colorFormat[5]; // ABGR
};

#define POSTHANDLER_START() \
          if (server.args() >= 1)\
          {\
            const char* input = server.arg(0).c_str();\
            char json[100];\
            url_decode(json, input, sizeof(json));\
            logln(server.arg(0));\
            logln("decoded:");\
            logln(json);

#define JSONPARSE_START(json) \
            struct jsonparse_state jsonState;\
            jsonparse_setup(&jsonState, json, strlen(json));\
            byte type;\
            bool error = false;\
            while (type = jsonparse_next(&jsonState))\
            {\
              logln(String(type));\
              if (type == JSON_TYPE_PAIR_NAME)\
              {\
                char key[30], data[50];\
                jsonparse_copy_value(&jsonState, key, sizeof(key));\
                logln("found key: ");\
                logln(key);\
                if (!AssertParseNext(&jsonState, JSON_TYPE_PAIR))\
                {\
                  error=true;\
                  break;\
                }\
                if (!AssertParseNext(&jsonState, JSON_TYPE_STRING)) break;\
                jsonparse_copy_value(&jsonState, data, sizeof(data));



// jsonparse_next(&jsonState); \ //dummy parse data of pair TODO: check for int, bool, or string)



#define JSONPARSE_END()   } }

#define POSTHANDLER_END() \
            if (!error)\
            {\
              server.send ( 200, "text/plain", "OK" );\
            }\
            else\
            {\
              server.send ( 500, "text/plain", "argument invalid!");\
            }\
          }\
          else\
          {\
            server.send ( 500, "text/plain", "argument missing!");\
          }

class SettingsManager
{
private:
  const String apConfigPath = "settings/ap.bin";
  const String apServerConfigPath = "settings/server1.bin";
  const String apListConfigPath = "settings/aplist1.bin";
  const String preferedAPConfigPath = "settings/preferedap.bin";

  // used in resetAPList & getNextAP
  int apListIndex = -1;
  FSFile apListFile;

public:
  bool getServerConfig(struct ServerConfig *config)
  {
    String path = apServerConfigPath;
    if(FS.exists((char *)path.c_str()))
    {
      FSFile file = FS.open((char *)path.c_str());
      file.read(config, sizeof(*config));
      file.close();
      return true;
    }
    safeStrncpy(config->hostname, "magicshifter", sizeof(config->hostname));
    config->port = 80;
    return false;
  }

  void setServerConfig(struct ServerConfig *config)
  {
    String path = apServerConfigPath;
    FSFile file = FS.open((char *)path.c_str(), FSFILE_OVERWRITE);
    file.write((uint8_t *)config, sizeof(*config));
    file.close();
  }

  bool getAPConfig(struct APInfo *config)
  {
    String path = apConfigPath;
    if(FS.exists((char *)path.c_str()))
    {
      FSFile file = FS.open((char *)path.c_str());
      file.read(config, sizeof(*config));
      file.close();
      return true;
    }
    safeStrncpy(config->ssid, "MS3000", sizeof(config->ssid));
    safeStrncpy(config->password, "", sizeof(config->password));
    return false;
  }

  void setAPConfig(struct APInfo *config)
  {
    String path = apConfigPath;
    FSFile file = FS.open((char *)path.c_str(), FSFILE_OVERWRITE);
    file.write((uint8_t *)config, sizeof(*config));
    file.close();

    logln("saved:");
    logln(config->ssid);
  }

  bool getPreferedAP(struct APInfo *config)
  {
    String path = preferedAPConfigPath;
    if(FS.exists((char *)path.c_str()))
    {
      FSFile file = FS.open((char *)path.c_str());
      file.read(config, sizeof(*config));
      file.close();
      return true;
    }
    safeStrncpy(config->ssid, "", sizeof(config->ssid));
    safeStrncpy(config->password, "", sizeof(config->password));
    return false;
  }

  void setPreferedAP(struct APInfo *config)
  {
    String path = preferedAPConfigPath;
    FSFile file = FS.open((char *)path.c_str(), FSFILE_OVERWRITE);
    file.write((uint8_t *)config, sizeof(*config));
    file.close();
  }

  void deleteAP(char *ssid)
  {
    String path = apListConfigPath;
    // bug in FS WRITE define!!!!
    FSFile apListFile = FS.open((char *)path.c_str(), (SPIFFS_RDONLY | SPIFFS_WRONLY | SPIFFS_CREAT));
    APInfo apInfoDummy;
    const int requiredBytes = sizeof(apInfoDummy);
    int apListIndex = 0;

    int lastPos = apListFile.position();

    while (apListFile.read(&apInfoDummy, requiredBytes) == requiredBytes)
    {
      if (strcmp(apInfoDummy.ssid, ssid) == 0)
      {
        log("deleting wifi:", VERBOSE);
        logln(ssid, VERBOSE);

        apInfoDummy.clear();
        int calcPos = apListIndex * requiredBytes;
        apListFile.seek(calcPos);
        apListFile.write((uint8_t *)&apInfoDummy, requiredBytes);
        break;
      }
      apListIndex++;
      lastPos = apListFile.position();
    }
    apListFile.close();
  }

  void addAP(struct APInfo *apInfo)
  {
    String path = apListConfigPath;
    FSFile apListFile = FS.open((char *)path.c_str(), FSFILE_READ);
    const int requiredBytes = sizeof(*apInfo);
    APInfo apInfoDummy;
    int apListIndex = 0;
    int firstFreePos = -1;

    while (apListFile.read(&apInfoDummy, requiredBytes) == requiredBytes)
    {
      if (firstFreePos < 0 && memcmpByte((byte *)&apInfoDummy, 0, requiredBytes))
      {
        firstFreePos = apListIndex * requiredBytes;
      }
      else if (strcmp(apInfoDummy.ssid, apInfo->ssid) == 0)
      {
        firstFreePos = apListIndex * requiredBytes;
        break;
      }
      apListIndex++;
    }
    apListFile.close();

    if (firstFreePos >= 0)
    {
      logln("found hole!");
      logln(String(firstFreePos));
      apListFile = FS.open((char *)path.c_str(), (SPIFFS_RDONLY | SPIFFS_WRONLY | SPIFFS_CREAT));
      apListFile.seek(firstFreePos);
    }
    else
    {
      logln("appendiong at end");
      apListFile = FS.open((char *)path.c_str(), FSFILE_WRITE);
    }
    apListFile.write((uint8_t *)apInfo, requiredBytes);
    apListFile.close();
  }

  void resetAPList()
  {
      apListIndex = -1;
      apListFile.close();
  }

  bool getNextAP(struct APInfo *apInfo)
  {
    if (apListIndex < 0)
    {
      String path = apListConfigPath;
      if(FS.exists((char *)path.c_str()))
      {
        apListFile = FS.open((char *)path.c_str());
        apListIndex = 0;
      }
    }

    if (apListIndex >= 0)
    {
      const int requiredBytes = sizeof(*apInfo);
      do
      {
        if (apListFile.read(apInfo, requiredBytes) == requiredBytes)
        {
          apListIndex++;
          if (!memcmpByte((byte *)apInfo, 0, requiredBytes))
            return true;
        }
        else
        {
          return false;
        }
      } while(true);
    }
    else
    {
      return false;
    }
  }
};
SettingsManager Settings;

bool parseAPInfoFromServerArgs(APInfo &apInfo)
{
  bool error = false;
  for (int i = 0; i < server.args(); i++)
  {
    logln("argName: ", VERBOSE);
    logln(server.argName(i), VERBOSE);

    logln("arg: ", VERBOSE);
    logln(server.arg(i), VERBOSE);

    if (strcmp(server.argName(i).c_str(), "ssid") == 0)
    {
      safeStrncpy(apInfo.ssid, server.arg(i).c_str(), sizeof(apInfo.ssid));
    }
    else if (strcmp(server.argName(i).c_str(), "pwd") == 0)
    {
      safeStrncpy(apInfo.password, server.arg(i).c_str(), sizeof(apInfo.password));
    }
    else
    {
      error = true;
    }
  }
  return error;
}

void handleGETServerSettings(void)
{
  logln("handleGETServerSettings", INFO);

  ServerConfig config;
  Settings.getServerConfig(&config);

  String response = "{";
  response += "\"host\":";
  response += "\"";
  response += config.hostname;
  response += "\"";
  response += ",";
  response += "\"port\":";
  response += config.port;
  response += "}";
  server.send(200, "text/plain", response);
}

void handlePOSTServerSettings(void)
{
  logln("handlePOSTServerSettings", INFO);
  if (server.args() >= 2)
  {
    ServerConfig config;
    Settings.getServerConfig(&config);

    bool error = false;
    for (int i = 0; i < server.args(); i++)
    {
      logln("argName: ", VERBOSE);
      logln(server.argName(i), VERBOSE);

      logln("arg: ", VERBOSE);
      logln(server.arg(i), VERBOSE);

      if (strcmp(server.argName(i).c_str(), "host") == 0)
      {
        safeStrncpy(config.hostname, server.arg(i).c_str(), sizeof(config.hostname));
      }
      else if (strcmp(server.argName(i).c_str(), "port") == 0)
      {
        config.port = atoi(server.arg(i).c_str());
      }
      else
      {
        error = true;
      }
    }

    if (!error)
    {
      Settings.setServerConfig(&config);
      server.send (200, "text/plain", "OK");
    }
    else
    {
      server.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    server.send ( 500, "text/plain", "argument missing!");
  }
}

void handleGETAPSettings(void)
{
  logln("handleGETAPSettings");

  APInfo apInfo;
  Settings.getAPConfig(&apInfo);

  String response = "{";
  response += "\"ssid\":";
  response += "\"";
  response += apInfo.ssid;
  response += "\"";
  //response += ",";
  //response += "\"pwd\":";
  //response += "\"" + apInfo.pwd + "\"";
  response += "}";
  server.send(200, "text/plain", response);
}

void handlePOSTAPSettings(void)
{
  logln("handlePOSTAPSettings", INFO);

  if (server.args() >= 2)
  {
    // load old settings
    APInfo apInfo;
    Settings.getAPConfig(&apInfo);

    bool error = false;
    for (int i = 0; i < server.args(); i++)
    {
      logln("argName: ", VERBOSE);
      logln(server.argName(i), VERBOSE);

      logln("arg: ", VERBOSE);
      logln(server.arg(i), VERBOSE);

      if (strcmp(server.argName(i).c_str(), "ssid") == 0)
      {
        safeStrncpy(apInfo.ssid, server.arg(i).c_str(), sizeof(apInfo.ssid));
      }
      else if (strcmp(server.argName(i).c_str(), "pwd") == 0)
      {
        safeStrncpy(apInfo.password, server.arg(i).c_str(), sizeof(apInfo.password));
      }
      else
      {
        error = true;
      }
    }

    if (!error)
    {
      logln("saving setAPConfig");
      Settings.setAPConfig(&apInfo);
      server.send (200, "text/plain", "OK");
    }
    else
    {
      server.send(500, "text/plain", "unknown args!");
    }
  }
  else
  {
    server.send ( 500, "text/plain", "argument missing!");
  }
}


void handleGETPreferdAPSettings(void)
{
  logln("handleGETPreferdAPSettings", INFO);

  APInfo apInfo;
  Settings.getPreferedAP(&apInfo);

  String response = "{";
  response += "\"ssid\":";
  response += "\"";
  response += apInfo.ssid;
  response += "\"";
  //response += ",";
  //response += "\"pwd\":";
  //response += "\"" + apInfo.pwd + "\"";
  response += "}";
  server.send(200, "text/plain", response);
}

void handlePOSTPreferedAPSettings(void)
{
  logln("handlePOSTPreferedAPSettings", INFO);

  if (server.args() >= 2)
  {
    APInfo apInfo;
    Settings.getPreferedAP(&apInfo);

    bool error = false;
    for (int i = 0; i < server.args(); i++)
    {
      logln("argName: ", VERBOSE);
      logln(server.argName(i), VERBOSE);

      logln("arg: ", VERBOSE);
      logln(server.arg(i), VERBOSE);

      if (strcmp(server.argName(i).c_str(), "ssid") == 0)
      {
        safeStrncpy(apInfo.ssid, server.arg(i).c_str(), sizeof(apInfo.ssid));
      }
      else if (strcmp(server.argName(i).c_str(), "pwd") == 0)
      {
        safeStrncpy(apInfo.password, server.arg(i).c_str(), sizeof(apInfo.password));
      }
      else
      {
        error = true;
      }
    }

    if (!error)
    {
      logln("saving setAPConfig");
      Settings.setPreferedAP(&apInfo);
      server.send (200, "text/plain", "OK");
    }
    else
    {
      server.send(500, "text/plain", "unknown args!");
    }
  }
  else
  {
    server.send ( 500, "text/plain", "argument missing!");
  }
}


///////////////////////////////////////
//////////////////////////////////////////

void handleGETAPList(void)
{
  logln("handleGETAPList", INFO);

  APInfo apInfo;
  Settings.getPreferedAP(&apInfo);

  String response = "[";

  Settings.resetAPList();
  bool firstAP = true;
  while (Settings.getNextAP(&apInfo))
  {
    if (!firstAP)
    {
      response += ",";
    }
    firstAP = false;

    response += "{\"ssid\":";
    response += "\"";
    response += apInfo.ssid;
    response += "\"";
    //response += ",";
    //response += "\"pwd\":";
    //response += "\"" + apInfo.pwd + "\"";
    response += "}";
  }
  response += "]";


  Settings.resetAPList();

  server.send(200, "text/plain", response);
}

void handlePOSTAPListAdd(void)
{
  logln("handlePOSTAPListAdd", INFO);

  if (server.args() >= 2)
  {
    APInfo apInfo;
    memset(apInfo.ssid, 0, sizeof(apInfo.ssid));
    memset(apInfo.password, 0, sizeof(apInfo.password));

    if (!parseAPInfoFromServerArgs(apInfo))
    {
      if (!strcmp(apInfo.ssid, "") == 0)
      {
        logln("adding wifi");
        Settings.addAP(&apInfo);
      }
      server.send (200, "text/plain", "OK");
    }
    else
    {
      server.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    server.send(500, "text/plain", "args missing!");
  }
}

void handlePOSTAPSListDelete(void)
{
  logln("handlePOSTAPSListDelete", INFO);

  if (server.args() >= 1)
  {
    APInfo apInfo;
    strcpy(apInfo.ssid, "");
    strcpy(apInfo.password, "");

    if (!parseAPInfoFromServerArgs(apInfo))
    {
      if (!strcmp(apInfo.ssid, "") == 0)
      {
        logln("deleting wifi");
        Settings.deleteAP(apInfo.ssid);
      }
      server.send (200, "text/plain", "OK");
    }
    else
    {
      server.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    server.send(500, "text/plain", "args missing!");
  }
}