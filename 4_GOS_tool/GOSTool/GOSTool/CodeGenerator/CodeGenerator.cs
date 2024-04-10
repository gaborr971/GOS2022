using System;
using System.Collections.Generic;
using System.IO;

namespace GOSTool
{
    public static class CodeGenerator
    {
        private static Dictionary<string, Func<ModuleConfig, string>> templateReplaceDelegates = new Dictionary<string, Func<ModuleConfig, string>>
        {
            ["#source_name"                ] = CreateSourceName,
            ["#author"                     ] = CreateAuthor,
            ["#date"                       ] = CreateDate,
            ["#year"                       ] = CreateYear,
            ["#header_name"                ] = CreateHeaderName,
            ["#source_includes"            ] = CreateSourceIncludes,
            ["#header_includes"            ] = CreateHeaderIncludes,
            ["#source_macros"              ] = CreateSourceMacros,
            ["#header_macros"              ] = CreateHeaderMacros,
            ["#source_typedefs"            ] = CreateSourceTypedefs,
            ["#header_typedefs"            ] = CreateHeaderTypedefs,
            ["#static_variables"           ] = CreateSourceStaticVariables,
            ["#function_prototypes"        ] = CreateSourceFunctionPrototypes,
            ["#taskdescriptors"            ] = CreateTaskDescriptors,
            ["#initfunction_name"          ] = CreateInitFunctionName,
            ["#initresult"                 ] = CreateInitResult,
            ["#initfunction_initialization"] = CreateInitFunctionInitialization,
            ["#taskfunction_bodies"        ] = CreateTaskFunctionBodies,
            ["#header_guard"               ] = CreateHeaderGuard
        };

        public static void GenerateApplicationFiles ()
        {
            ProjectData projectData = ProjectHandler.GetProjectData();
            string projectFolder = ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build\\" + projectData.AppConfig.Name;

            foreach (var moduleConfig in projectData.AppConfig.Modules)
            {
                // Create module folders.
                if (Directory.Exists(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName))
                {
                    Directory.Delete(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName, true);
                }
                Directory.CreateDirectory(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName);
                Directory.CreateDirectory(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName + "\\src");
                Directory.CreateDirectory(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName + "\\inc");

                // Read template.
                string moduleSourceFile = "";
                string moduleHeaderFile = "";

                using (StreamReader srSource = File.OpenText(ProgramData.SourceTemplatePath))
                using (StreamReader srHeader = File.OpenText(ProgramData.HeaderTemplatePath))
                {
                    moduleSourceFile = srSource.ReadToEnd();
                    moduleHeaderFile = srHeader.ReadToEnd();
                }

                // Fill out source template.
                moduleSourceFile = FillModuleSourceTemplate(moduleConfig, moduleSourceFile);

                // Fill out header template.
                moduleHeaderFile = FillModuleHeaderTemplate(moduleConfig, moduleHeaderFile);

                // Write module files.
                using (StreamWriter swSource = new StreamWriter(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName + "\\src\\" + moduleConfig.ModuleName.ToLower() + ".c"))
                using (StreamWriter swHeader = new StreamWriter(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName + "\\inc\\" + moduleConfig.ModuleName.ToLower() + ".h"))
                {

                    swSource.Write(moduleSourceFile);
                    swHeader.Write(moduleHeaderFile);
                }
            }
        }

        private static string FillModuleSourceTemplate (ModuleConfig moduleConfig, string moduleSourceFile)
        {
            // Modify fields.
            foreach (KeyValuePair<string, Func<ModuleConfig, string>> entry in templateReplaceDelegates)
            {
                moduleSourceFile = moduleSourceFile.Replace(entry.Key, entry.Value(moduleConfig));
            }

            return moduleSourceFile;
        }

        private static string FillModuleHeaderTemplate(ModuleConfig moduleConfig, string moduleHeaderFile)
        {
            // Modify fields.
            foreach (KeyValuePair<string, Func<ModuleConfig, string>> entry in templateReplaceDelegates)
            {
                moduleHeaderFile = moduleHeaderFile.Replace(entry.Key, entry.Value(moduleConfig));
            }

            return moduleHeaderFile;
        }

        private static string CreateHeaderTypedefs(ModuleConfig moduleConfig)
        {
            return "";
        }        
        
        private static string CreateHeaderMacros(ModuleConfig moduleConfig)
        {
            return "";
        }

        private static string CreateHeaderIncludes(ModuleConfig moduleConfig)
        {
            return "#include \"gos.h\"\r\n";
        }

        private static string CreateHeaderGuard(ModuleConfig moduleConfig)
        {
            string headerGuard = "";
            headerGuard += "#ifndef " + moduleConfig.ModuleName.ToUpper() + "_H\r\n";
            headerGuard += "#define " + moduleConfig.ModuleName.ToUpper() + "_H\r\n";
            return headerGuard;
        }

        private static string CreateTaskFunctionBodies(ModuleConfig moduleConfig)
        {
            string taskFunctionBodies = "";

            foreach(var taskConfig in moduleConfig.Tasks)
            {
                taskFunctionBodies += "/**\r\n * @brief   TODO\r\n * @details TODO\r\n *\r\n * @return  -\r\n */\r\n";
                taskFunctionBodies += "GOS_STATIC void_t " + ConvertTaskNameToFunctionName(taskConfig.TaskName) + " (void_t)\r\n";
                taskFunctionBodies += "{\r\n";

                // Local variables.
                taskFunctionBodies += "    /*\r\n";
                taskFunctionBodies += "     * Local variables.\r\n";
                taskFunctionBodies += "     */\r\n";
                taskFunctionBodies += "\r\n";

                // Task initializer / startup activity.
                taskFunctionBodies += "    // Task initializer code.\r\n";

                if (taskConfig.TaskType == TaskType.Startup || taskConfig.TaskType == TaskType.StartupSingleShot)
                {
                    taskFunctionBodies += "    // Startup activity here.\r\n";
                }

                taskFunctionBodies += "\r\n";

                // Priority handling.
                if (taskConfig.TaskType == TaskType.Startup)
                {
                    taskFunctionBodies += "    // Restore priority for further operation.\r\n";
                    taskFunctionBodies += "    // Fake ISR section to make sure there is no privielege issue.\r\n";
                    taskFunctionBodies += "    GOS_ISR_ENTER\r\n";
                    taskFunctionBodies += "    (void_t) gos_taskSetPriority(" + GetTaskIdVariableName(taskConfig) + ", " + taskConfig.TaskPriority + ");\r\n";
                    taskFunctionBodies += "    GOS_ISR_EXIT\r\n";
                    taskFunctionBodies += "\r\n";
                }


                // Function code.
                taskFunctionBodies += "    /*\r\n";
                taskFunctionBodies += "     * Function code.\r\n";
                taskFunctionBodies += "     */\r\n";
                
                // Task loop.
                taskFunctionBodies += "    for (;;)\r\n";
                taskFunctionBodies += "    {\r\n";

                if (taskConfig.TaskType == TaskType.SingleShot || taskConfig.TaskType == TaskType.StartupSingleShot)
                {
                    taskFunctionBodies += "        // Delete task from scheduling as it is single-shot.\r\n";
                    taskFunctionBodies += "        (void_t) gos_taskDelete(" + GetTaskDescriptorName(taskConfig) + ".taskId);\r\n";
                }
                else
                {
                    taskFunctionBodies += "        // Task function main activity here.\r\n";
                }
                
                taskFunctionBodies += "    }\r\n";
                taskFunctionBodies += "}\r\n\r\n";
            }

            return taskFunctionBodies;
        }

        private static string CreateInitFunctionInitialization(ModuleConfig moduleConfig)
        {
            string initCode = "";

            initCode += "if (";

            if (moduleConfig.Tasks.Count == 1)
            {
                initCode += "gos_taskRegister(&" + GetTaskDescriptorNames(moduleConfig)[0] + ", &" + GetTaskIdVariableNames(moduleConfig)[0] + ") != GOS_SUCCESS)\r\n";
            }
            else
            {
                for (int i = 0; i < moduleConfig.Tasks.Count - 1; i++)
                {
                    initCode += "gos_taskRegister(&" + GetTaskDescriptorNames(moduleConfig)[i] + ", &" + GetTaskIdVariableNames(moduleConfig)[i] + ") != GOS_SUCCESS) ||\r\n        ";
                }
                initCode += "gos_taskRegister(&" + GetTaskDescriptorNames(moduleConfig)[moduleConfig.Tasks.Count - 1] + ", &" + GetTaskIdVariableNames(moduleConfig)[moduleConfig.Tasks.Count - 1] + ") != GOS_SUCCESS)\r\n";
            }

            initCode += "    {\r\n";
            initCode += "        " + CreateInitResult(moduleConfig) + " = GOS_ERROR;\r\n";
            initCode += "    }\r\n";
            initCode += "    else\r\n";
            initCode += "    {\r\n";
            initCode += "        // Nothing to do.\r\n";
            initCode += "    }";

            return initCode;
        }

        private static string CreateInitResult(ModuleConfig moduleConfig)
        {
            return CreateInitFunctionName(moduleConfig) + "Result";
        }

        private static string CreateInitFunctionName (ModuleConfig moduleConfig)
        {
            return ConvertTaskNameToFunctionName(moduleConfig.ModuleName) + "Init";
        }

        private static string CreateSourceMacros (ModuleConfig moduleConfig)
        {
            return "";
        }        
        
        private static string CreateSourceTypedefs(ModuleConfig moduleConfig)
        {
            return "";
        }

        private static string CreateSourceIncludes (ModuleConfig moduleConfig)
        {
            return "#include \"" + moduleConfig.ModuleName.ToLower() + ".h\"\r\n";
        }

        private static string CreateHeaderName (ModuleConfig moduleConfig)
        {
            return moduleConfig.ModuleName.ToLower() + ".h";
        }

        private static string CreateYear (ModuleConfig moduleConfig)
        {
            return DateTime.Now.ToString("yyyy");
        }

        private static string CreateDate (ModuleConfig moduleConfig)
        {
            return DateTime.Now.ToString("yyyy-MM-dd");
        }

        private static string CreateAuthor (ModuleConfig moduleConfig)
        {
            return ProgramData.Name + " " + ProgramData.Version;
        }

        private static string CreateSourceName (ModuleConfig moduleConfig)
        {
            return moduleConfig.ModuleName.ToLower() + ".c";
        }

        private static string CreateTaskDescriptors (ModuleConfig moduleConfig)
        {
            string taskDescriptors = "";

            foreach (var taskConfig in moduleConfig.Tasks)
            {
                taskDescriptors += "/**\r\n * " + ConvertTaskNameToComment(taskConfig.TaskName) + "task descriptor.\r\n */\r\n";
                taskDescriptors += "GOS_STATIC gos_taskDescriptor_t " + GetTaskDescriptorName(taskConfig) + " =\r\n";
                taskDescriptors += "{\r\n";
                taskDescriptors += "    .taskFunction       = " + ConvertTaskNameToFunctionName(taskConfig.TaskName) + ",\r\n";
                taskDescriptors += "    .taskName           = \"" + taskConfig.TaskName + "\",\r\n";
                taskDescriptors += "    .taskStackSize      = " + "0x" + taskConfig.TaskStackSize.ToString("X") + ",\r\n";

                if (taskConfig.TaskType == TaskType.Startup || taskConfig.TaskType == TaskType.StartupSingleShot)
                {
                    taskDescriptors += "    // Note: Task priority is set in task function for startup tasks.\r\n";
                    taskDescriptors += "    .taskPriority       = 0,\r\n";
                }
                else
                {
                    taskDescriptors += "    .taskPriority       = " + taskConfig.TaskPriority + ",\r\n";
                }
                taskDescriptors += "    .taskPrivilegeLevel = " + Helper.GetPrivilegeText(taskConfig.TaskPrivileges) + ",\r\n";
                taskDescriptors += "    .taskCpuUsageLimit  = " + taskConfig.TaskMaxCpu + "\r\n";
                taskDescriptors += "};\r\n";
            }

            return taskDescriptors;
        }

        private static List<string> GetTaskDescriptorNames (ModuleConfig moduleConfig)
        {
            List<string> taskDescriptorNames = new List<string>();

            foreach (var taskConfig in moduleConfig.Tasks)
            {
                taskDescriptorNames.Add(GetTaskDescriptorName(taskConfig));
            }

            return taskDescriptorNames;
        }

        private static string GetTaskDescriptorName (TaskConfig taskConfig)
        {
            return ConvertTaskNameToFunctionName(taskConfig.TaskName) + "Desc";
        }

        private static List<string> GetTaskIdVariableNames (ModuleConfig moduleConfig)
        {
            List<string> taskIdVariableNames = new List<string>();

            foreach (var taskConfig in moduleConfig.Tasks)
            {
                taskIdVariableNames.Add(GetTaskIdVariableName(taskConfig));
            }

            return taskIdVariableNames;
        }

        private static string GetTaskIdVariableName (TaskConfig taskConfig)
        {
            return taskConfig.TaskName.ToLower() + "_id";
        }

        private static string CreateSourceStaticVariables (ModuleConfig moduleConfig)
        {
            string staticVariables = "";

            foreach (var taskConfig in moduleConfig.Tasks)
            {
                staticVariables += "/**\r\n * " + ConvertTaskNameToComment(taskConfig.TaskName) + "task ID.\r\n */\r\n";
                staticVariables += "GOS_STATIC gos_tid_t " + taskConfig.TaskName.ToLower() + "_id;\r\n";
            }

            return staticVariables;
        }

        private static string CreateSourceFunctionPrototypes (ModuleConfig moduleConfig)
        {
            string functionPrototypes = "";

            foreach (var taskConfig in moduleConfig.Tasks)
            {
                functionPrototypes += "GOS_STATIC void_t " + ConvertTaskNameToFunctionName(taskConfig.TaskName) + " (void_t);\r\n";
            }

            return functionPrototypes;
        }

        private static string ConvertTaskNameToComment (string taskName)
        {
            string replaced = taskName.Replace("_", " ");
            string[] splitted = replaced.Split(' ');
            for (int i = 0; i < splitted.Length; i++)
            {
                splitted[i] = splitted[i].ToCharArray()[0].ToString().ToUpper() + splitted[i].Substring(1);
            }
            string built = "";
            foreach (var word in splitted)
            {
                built += word + " ";
            }
            return built;
        }

        private static string ConvertTaskNameToFunctionName (string taskName)
        {
            string replaced = taskName.Replace("_", " ");
            string[] splitted = replaced.Split(' ');
            for (int i = 0; i < splitted.Length; i++)
            {
                splitted[i] = splitted[i].ToCharArray()[0].ToString().ToUpper() + splitted[i].Substring(1);
            }
            string built = "";
            foreach (var word in splitted)
            {
                built += word;
            }
            return built;
        }
    }
}
