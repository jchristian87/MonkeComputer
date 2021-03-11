#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "custom-types/shared/register.hpp"
#include "GorillaUI.hpp"
#include "GorillaUI/MainView.hpp"
#include "GorillaUI/MainViewManager.hpp"
#include "Test/TestView.hpp"

#include "ViewLib/CustomComputer.hpp"
#include "Register.hpp"

#include "typedefs.h"
#include <vector>

using namespace GorillaUI;
using namespace GorillaUI::Components;

ModInfo modInfo;

Logger& getLogger()
{
    static Logger *logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)
#define run(value...) CRASH_UNLESS(il2cpp_utils::RunMethod(value))

std::string dataDir = "";

MAKE_HOOK_OFFSETLESS(GorillaComputer_Start, void, Il2CppObject* self)
{
    GorillaComputer_Start(self);

    Il2CppObject* computerGO = *il2cpp_utils::RunMethod(self, "get_gameObject");
    CustomComputer* computer = *il2cpp_utils::RunGenericMethod<CustomComputer*>(computerGO, "AddComponent", std::vector<Il2CppClass*>{classof(GorillaUI::CustomComputer*)});
    computer->Init(CreateView<MainView*>());
    getLogger().info("End of start");
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    INFO("Setup!");
}

extern "C" void load()
{
    INFO("Loading mod...");

    INSTALL_HOOK_OFFSETLESS(getLogger(), GorillaComputer_Start, il2cpp_utils::FindMethodUnsafe("", "GorillaComputer", "Start", 0));
    
    using namespace GorillaUI::Components;
    custom_types::Register::RegisterTypes<View, ViewManager>();
    custom_types::Register::RegisterTypes<MainViewManager, MainView>();
    custom_types::Register::RegisterType<TestView>();
    custom_types::Register::RegisterTypes<GorillaKeyboardButton, CustomComputer>();


    GorillaUI::Register::RegisterModView<TestView*>("TestView", "1.0.0");
    GorillaUI::Register::RegisterModView<TestView*>("TestView2", "1.0.0");
    INFO("Mod Loaded!");
}