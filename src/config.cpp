#include "config.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

config_t config;

extern Logger& getLogger();

static LoggerContextObject& getConfigLogger()
{
	static LoggerContextObject logger = getLogger().WithContext("Config");
	return logger;
}

#define INFO(...) getConfigLogger().info(__VA_ARGS__)
#define ERROR(...) getConfigLogger().error(__VA_ARGS__)

Configuration& getConfig() {
	static Configuration config({ID, VERSION});
	config.Load();
	return config;
}

#define SAVE(name) \
	doc.AddMember(#name, config.name, allocator)

#define LOAD(name, method) \
	auto itr_ ##name = doc.FindMember(#name); \
	if (itr_ ##name != doc.MemberEnd()) { \
		config.name = itr_ ##name->value.method; \
	}else { foundEverything = false; } 

void SaveConfig()
{
	INFO("Saving Configuration...");
	rapidjson::Document& doc = getConfig().config;
	doc.RemoveAllMembers();
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	
	SAVE(enableExtraSongDetails);
	SAVE(enableCustomSongColours);
	SAVE(enableCustomDiffNames);
	SAVE(enableBurnMarks);
	SAVE(openToCustomLevels);

	getConfig().Write();
	INFO("Saved Configuration!");
}

bool LoadConfig()
{
	INFO("Loading Configuration...");
	bool foundEverything = true;
	rapidjson::Document& doc = getConfig().config;

	LOAD(enableExtraSongDetails, GetBool());
	LOAD(enableCustomSongColours, GetBool());
	LOAD(enableCustomDiffNames, GetBool());
	LOAD(enableBurnMarks, GetBool());
	LOAD(openToCustomLevels, GetBool());

	if (!foundEverything) 
	{
		ERROR("Configuration values were missing!");
		return false;
	}
	INFO("Loaded Configuration!");
	return true;
}