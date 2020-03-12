using EVTechnology.Common.Helper;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Xml.Serialization;

namespace EVClassLib
{
    [Serializable]
    public class Scene : IFile
    {
        public Guid ID { set; get; }

        public string Name { set; get; }

        /// <summary>
        /// 工作文件夹路径
        /// </summary>
        public string WorkPath { set; get; }

        [XmlIgnore]
        public Icon Icon { set; get; }

        public List<ModuleData> ModuleDatas { set; get; }

        [XmlIgnore]
        public Dictionary<int, string> Setting { set; get; }

        public Scene()
        {
            ID = Guid.NewGuid();
            ModuleDatas = new List<ModuleData>();
        }

        public void Build()
        {
            Directory.CreateDirectory(WorkPath);
            FileHelper.CreateSerializeFile<Scene>(WorkPath + "\\" + Name + ".sst", this, FileType.XMLFile);
        }

        public void Save()
        {
            //生成.sst文件
            string sstPath = Path.Combine(WorkPath, Name + ".sst");
            FileHelper.CreateSerializeFile(sstPath, this, FileType.XMLFile);
            foreach (ModuleData md in this.ModuleDatas)
            {
                GetModule(md.ID).Save();
            }
        }

        public void Read()
        {

        }

        public void AddModule(Module mod)
        {
            //检测命名是否唯一
            if (ModuleDatas.Find(p => p.Name == mod.Name) != null)
                return;

            if (string.IsNullOrEmpty(mod.WorkPath))
            {
                mod.WorkPath = WorkPath + "//" + mod.Name + "//" + mod.Name + ".mt";
            }
            mod.Save();
            if (mod.GetType() == typeof(ExecModule))
                ((ExecModule)mod).Build();

            ModuleData data = new ModuleData(mod.ID, mod.Name, mod.WorkPath);
            ModuleDatas.Add(data);
            Save();
        }

        public void DeleteModule(Module mod)
        {
            mod.Delete();
            ModuleData data = ModuleDatas.Find(p => p.ID == mod.ID);
            if (data != null)
                ModuleDatas.Remove(data);

            Save();
        }

        public Module GetModule(Guid id)
        {
            ModuleData data = ModuleDatas.Find(p => p.ID == id);
            Module mod = FileHelper.AnalysisSerializeFile<Module>(data.WorkPath, FileType.BinaryFile);
            mod.WorkPath = data.WorkPath;      //存为了绝对路径
            return mod;
        }

        public Module GetModule(string name)
        {
            ModuleData data = ModuleDatas.Find(p => p.Name == name);
            if (data == null) return null;
            Module mod = FileHelper.AnalysisSerializeFile<Module>(data.WorkPath, FileType.BinaryFile);
            return mod;
        }

        public List<ExecModule> GetAllModule()
        {
            List<ExecModule> emods = new List<ExecModule>();
            foreach (var data in ModuleDatas)
            {
                Module mod = FileHelper.AnalysisSerializeFile<Module>(data.WorkPath, FileType.BinaryFile);
                if (mod.GetType() == typeof(ExecModule))
                    emods.Add((ExecModule)mod);
            }
            return emods;
        }
    }

    public class ModuleData
    {
        public Guid ID;

        public string Name;

        public string WorkPath { set; get; }

        public ModuleData()
        {

        }

        public ModuleData(Guid _ID, string _Name, string _WorkPath)
        {
            ID = _ID;
            Name = _Name;
            WorkPath = _WorkPath;
        }
    }
}
