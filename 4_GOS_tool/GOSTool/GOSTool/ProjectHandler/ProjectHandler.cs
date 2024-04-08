using GOSTool.DataHandler;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;

namespace GOSTool
{
    public class ProjectHandler
    {
        public static DataVariable<string> WorkspacePath { get; set; } = new DataVariable<string>(Directory.GetCurrentDirectory());
        public static DataVariable<string> ProjectName { get; set; } = new DataVariable<string>("New project");

        public static DataVariable<List<string>> ProjectNames { get; set; } = new DataVariable<List<string>>(new List<string>());

        private static List<string> ProjectPaths = new List<string>();

        public static DataVariable<string> SelectedProject { get; set; } = new DataVariable<string>();

        public static void GetProjectNames()
        {
            List<string> files = new List<string>();
            ProjectPaths.Clear();

            string[] directories = Directory.GetDirectories(WorkspacePath.Value);
            foreach(string dir in directories)
            {
                string[] _files = Directory.GetFiles(dir, "project.gos", SearchOption.TopDirectoryOnly);
                files.AddRange(_files);
                ProjectPaths.AddRange(_files);
            }

            List<ProjectData> projects = new List<ProjectData>();
            List<string> projectNames = new List<string>();

            foreach(string file in files)
            {
                var mySerializer = new XmlSerializer(typeof(ProjectData));
                var myFileStream = new FileStream(file, FileMode.Open);
                var myObject = (ProjectData)mySerializer.Deserialize(myFileStream);
                myFileStream.Close();
                projects.Add(myObject);
                projectNames.Add(myObject.ProjectName);
            }

            ProjectNames.SetValue(projectNames);
        }

        public static ProjectData GetProjectData()
        {
            foreach(string path in ProjectPaths)
            {
                var mySerializer = new XmlSerializer(typeof(ProjectData));
                var myFileStream = new FileStream(path, FileMode.Open);
                var myObject = (ProjectData)mySerializer.Deserialize(myFileStream);
                myFileStream.Close();
                
                if (myObject.ProjectName == SelectedProject.Value || myObject.ProjectName == ProjectName.Value)
                {
                    return myObject;
                }
            }

            return null;
        }

        public static void SaveProjectData(ProjectData projectData)
        {
            if (projectData.ProjectName != "")
            {
                projectData.LastModified = DateTime.Now;
                XmlSerializer ser = new XmlSerializer(typeof(ProjectData));
                TextWriter writer = new StreamWriter(WorkspacePath.Value + "/" + ProjectName.Value + "/" + "project.gos");
                ser.Serialize(writer, projectData);
                writer.Close();
            }
        }

        public static void CreateProject()
        {
            if (ProjectName.Value != "")
            {
                Directory.CreateDirectory(WorkspacePath.Value + "/" + ProjectName.Value);
                XmlSerializer ser = new XmlSerializer(typeof(ProjectData));
                ProjectData pd = new ProjectData();
                pd.ProjectName = ProjectName.Value;
                pd.Workspace = WorkspacePath.Value;
                TextWriter writer = new StreamWriter(WorkspacePath.Value + "/" + ProjectName.Value + "/" + "project.gos");
                ser.Serialize(writer, pd);
                writer.Close();
                GetProjectNames();
            }
            else
            {
                MessageBox.Show("Project name cannot be empty!", "Error", MessageBoxButtons.OK);
            }            
        }

        public static void OpenProject()
        {
            var projData = GetProjectData();
            projData.LastAccessed = DateTime.Now;
            SaveProjectData(projData);
        }
    }

    public class ProjectData
    {
        public DateTime CreationTime { get; set; }
        public DateTime LastModified { get; set; }
        public DateTime LastAccessed { get; set; }
        public string ProjectName { get; set; }
        public string Workspace { get; set; }
        public OSConfig OsConfig { get; set; } = new OSConfig();
        public ProjectData()
        {
            CreationTime = DateTime.Now;
            LastModified = DateTime.Now;
            LastAccessed = DateTime.Now;
        }
    }

    public class OSConfig
    {
        public string Version { get; set; } = "";
        public List<(string, string)> Configuration { get; set; } = new List<(string, string)>();
    }
}
