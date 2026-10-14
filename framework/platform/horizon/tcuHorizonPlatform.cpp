/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright (c) 2026 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief Headless Horizon platform.
 *//*--------------------------------------------------------------------*/

#include "tcuFunctionLibrary.hpp"
#include "tcuPlatform.hpp"
#include "vkPlatform.hpp"

#include <switch.h>
#include <cstring>

// CTS uses its own Vulkan types; only the loader entry point is linked directly.
extern "C" vk::PFN_vkVoidFunction vkGetInstanceProcAddr(vk::VkInstance instance, const char *name);

namespace
{

class VulkanFunctionLibrary : public tcu::FunctionLibrary
{
public:
    deFunctionPtr getFunction(const char *name) const
    {
        if (std::strcmp(name, "vkGetInstanceProcAddr") == 0)
            return reinterpret_cast<deFunctionPtr>(vkGetInstanceProcAddr);
        return reinterpret_cast<deFunctionPtr>(vkGetInstanceProcAddr(nullptr, name));
    }
};

class VulkanLibrary : public vk::Library
{
public:
    VulkanLibrary() : m_driver(m_library)
    {
    }

    const vk::PlatformInterface &getPlatformInterface() const
    {
        return m_driver;
    }

    const tcu::FunctionLibrary &getFunctionLibrary() const
    {
        return m_library;
    }

private:
    const VulkanFunctionLibrary m_library;
    const vk::PlatformDriver m_driver;
};

class VulkanPlatform : public vk::Platform
{
public:
    vk::Library *createLibrary(const char *libraryPath) const
    {
        if (libraryPath != nullptr)
            TCU_THROW(NotSupportedError, "Horizon uses a statically linked Vulkan driver");
        return new VulkanLibrary();
    }

    void describePlatform(std::ostream &dst) const
    {
        dst << "OS: Horizon\nCPU: AArch64\n";
    }
};

class Platform : public tcu::Platform
{
public:
    Platform()
    {
        consoleInit(nullptr);
    }

    ~Platform()
    {
        consoleExit(nullptr);
    }

    bool processEvents()
    {
        consoleUpdate(nullptr);
        return appletMainLoop();
    }

    const vk::Platform &getVulkanPlatform() const
    {
        return m_vulkanPlatform;
    }

private:
    VulkanPlatform m_vulkanPlatform;
};

} // anonymous namespace

tcu::Platform *createPlatform()
{
    return new Platform();
}
