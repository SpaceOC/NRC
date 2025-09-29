// Требуется переделать тест
/*
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <memory>
#include "Core/filesystem/nrfs.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/main.h"
#include "Core/modules/module_metadata.h"
#include "Core/modules/module_base.h"
#include "Core/other/versionc.h"
#include "Core/print/print.h"

class MyTestClass {
public:
    MyTestClass() {}
    ~MyTestClass() {}
    std::string getResult() { return "Hello, World!"; }
};

MyTestClass MTS;
MyTestClass* myTestClass() {
    return &MTS;
}

void* reqPtrClass() {
    return (void*)myTestClass();
}

int code = 0;

class Module : public core::ModuleBase {
public:
    Module(core::ModuleMetadata* m, core::main* c) : core::ModuleBase(m, c) {};
    Module() : core::ModuleBase() {}
    ~Module() {};
    bool runRequest(const std::string&) override { return false; }
    void onLoad() override {}
    void onUnload() override {}
    void onWrongRequest(int) override {}
    std::string usingTestClass() {
        auto a = (MyTestClass*)reqPtrClass();
        return a->getResult();
    }
};

TEST(MODULES, requestPtrClass) {
    std::string name = MTS.getResult();
    core::ModuleMetadata* metadata = new core::ModuleMetadata();
    metadata->name = metadata->description = "One" + name;
    metadata->uses[0] = metadata->uses[1] = new core::VersionC("0.5");

    std::unique_ptr<Module> moduleOne = std::make_unique<Module>();
    std::string a = moduleOne->usingTestClass();
    ASSERT_EQ(name, a);
}
*/