#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "OrbsWiFiManager.h"
#include "WifiManagerCustomParameters.h"
#include <Preferences.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#define CM_PARAM_TYPE_STRING 0
#define CM_PARAM_TYPE_INT 1
#define CM_PARAM_TYPE_BOOL 2
#define CM_PARAM_TYPE_FLOAT 3
#define CM_PARAM_TYPE_COLOR 4
#define CM_PARAM_TYPE_COMBOBOX 5

// Uncomment to show debug output
// #define CM_DEBUG

class ConfigManager {
public:
    ConfigManager(WiFiManager &wm);
    ~ConfigManager();

    static ConfigManager *getInstance();

    void setupWebPortal();
    void saveAllConfigs();

    // Register different types of variables with descriptions
    void addConfigString(const char *section, const char *varName, std::string *var, size_t length, const char *description);
    void addConfigInt(const char *section, const char *varName, int *var, const char *description);
    void addConfigFloat(const char *section, const char *varName, float *var, const char *description);
    void addConfigIP(const char *section, const char *varName, IPAddress *var, const char *description);
    void addConfigBool(const char *section, const char *varName, bool *var, const char *description);
    void addConfigColor(const char *section, const char *varName, int *var, const char *description);
    void addConfigComboBox(const char *section, const char *varName, int *var, String options[], int numOptions, const char *description);

    // Get stored values
    std::string getConfigString(const char *varName, std::string defaultValue);
    bool getConfigBool(const char *varName, bool defaultValue);
    int getConfigInt(const char *varName, int defaultValue);
    float getConfigFloat(const char *varName, float defaultValue);

    // Register callbacks for changes
    void addOnChangeCallback(
        const char *section,
        const char *varName,
        const std::function<void(const char *section, const char *varName)> &callback);
    void addOnChangeCallback(
        const char *section,
        const std::function<void(const char *section, const char *varName)> &callback);

    // Check if a restart is required
    bool isRequiresRestart() { return requiresRestart; }

private:
    struct Parameter {
        WiFiManagerParameter *parameter;
        const int type;
        const char *section;
        const char *variableName;
        std::function<void()> saveCallback;
    };

    static ConfigManager *s_instance;

    WiFiManager &m_wm;
    Preferences preferences;
    std::vector<Parameter> parameters;
    std::unordered_map<std::string, std::vector<std::function<void(const char *section, const char *varName)>>> changeCallbacks;
    bool requiresRestart = false;

    template <typename T, typename ParameterType, typename... Args>
    void addConfig(int paramType, const char *section, const char *varName, T *var, const char *description, uint8_t length,
                   std::function<void(T &)> loadFromPreferences, std::function<void(ParameterType *, T &)> setParameterValue, std::function<void(T &)> saveToPreferences, Args... args);

    std::string makeKey(const char *section, const char *varName);
    void triggerChangeCallbacks(const char *section, const char *varName = "");
};

#endif // CONFIG_MANAGER_H
