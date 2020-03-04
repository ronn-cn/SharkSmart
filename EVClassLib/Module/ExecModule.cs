using EVTechnology.Common.Controls;
using EVTechnology.Common.Helper;
using System;
using System.Collections.Generic;
using System.IO;

namespace EVClassLib
{
    [Serializable]
    public class ExecModule : Module
    {
        public string PresetName { set; get; }

        public List<Unit> Units { set; get; }

        public List<Plan> Plans { set; get; }

        public List<Resource> Resources { set; get; }

        public DataBase Data { get; set; }

        public string OutputPath { get; set; } = "\\Debug";

        public Dictionary<string, object> Setting { get; set; }

        private string FolderPath { get { return Path.GetDirectoryName(WorkPath); } }

        public ExecModule()
        {
            ID = Guid.NewGuid();
            Units = new List<Unit>();
            Plans = new List<Plan>();
            Resources = new List<Resource>();
            Data = new DataBase();
            Data.Datas.Add(new EVClassLib.Data("version", "20191224"));
            Resources = new List<Resource>();
            Setting = new Dictionary<string, object>();

            int[] v = { 1, 11, 111, 111 };
            Setting.Add("version", v);
        }

        public int GetNameCount(string newName)
        {
            int count = 0;
            count += Plans.FindAll(p => p.Name == newName).Count;
            count += Units.FindAll(p => p.Name == newName).Count;
            count += Units.FindAll(p => p.Name == newName).Count;
            return count;
        }

        /// <summary>
        /// 修改名称
        /// </summary>
        /// <param name="mod"></param>
        /// <param name="newName"></param>
        public void ModuleFileRename(Plan mod, string newName)
        {

        }

        /// <summary>
        /// 单元列表添加
        /// </summary>
        public void AddUnit(Unit unit)
        {
            if (Units.Find(p => p.Name == unit.Name) != null) return;
            unit.Parent = this;
            this.Units.Add(unit);
            string defaultUnitCode = @"def Init():
    pass
";
            if (unit.GetType() == typeof(UIUnit))
            {
                foreach (var page in ((UIUnit)unit).Pages)
                {
                    File.WriteAllText(GetFilePath(3, page.Name), "");
                }
            }
            File.WriteAllText(GetFilePath(1, unit.Name), defaultUnitCode);
            Save();
        }

        public void AddPlan(Plan plan)
        {
            if (Plans.Find(p => p.Name == plan.Name) != null) return;
            Plans.Add(plan);
            string defaultPlanCode = @"def Init():
    pass
";
            File.WriteAllText(GetFilePath(2, plan.Name), defaultPlanCode);
            Save();
        }

        public void AddRes(Resource res)
        {
            if (Resources == null) Resources = new List<Resource>();
            if (Resources.Find(p => p.Name == res.Name) != null) return;
            string ext = Path.GetExtension(res.Path);
            File.Copy(res.Path, Path.Combine(FolderPath, "Resources", res.Name + ext));
            res.Path = "\\Resources\\" + res.Name + ext;
            Resources.Add(res);
            Save();
        }

        //创建文件夹目录
        public void Build()
        {
            Directory.CreateDirectory(FolderPath + "\\" + "Units");
            Directory.CreateDirectory(FolderPath + "\\" + "Units\\Pages");
            Directory.CreateDirectory(FolderPath + "\\" + "Plans");
            Directory.CreateDirectory(FolderPath + "\\" + "Resources");
        }

        protected void GetViewData()
        {
            foreach (var unit in Units)
            {
                if (unit.GetType() != typeof(UIUnit))
                    continue;

                List<Canvas> datas = new List<Canvas>();
                foreach (var page in ((UIUnit)unit).Pages)
                {
                    string path = Path.Combine(GetFilePath(3, page.Name));
                    if (string.IsNullOrEmpty(File.ReadAllText(path))) return;
                    datas = FileHelper.AnalysisSerializeFile<List<Canvas>>(path, FileType.XMLFile);
                    List<SControl> tmps = new List<SControl>();
                    tmps.AddRange(page.Controls);
                    page.Controls.Clear();
                    foreach (var block in datas[0].Blocks)
                    {
                        SControl ctrl = BlockConvertHelper.ConvertClass(block);
                        if (ctrl.Name != page.Name) { ctrl.Name = page.Name + "_" + ctrl.Name; }
                        BaseEvent be = tmps.Find(p => p.Name == ctrl.Name)?.Event;
                        if (be != null) ctrl.Event = be;
                        page.Controls.Add(ctrl);

                        AddControlRes(block);
                    }
                }
            }
        }

        private void AddControlRes(UIBlock block)
        {
            if (block.GetType() == typeof(UIImage))
            {
                UIImage img = block as UIImage;
                AddResource(img.XMLImagePath);
            }
            else if (block.GetType() == typeof(UIIconView))
            {
                UIIconView img = block as UIIconView;
                foreach (var item in img.Items)
                {
                    AddResource(item.XMLImagePath);
                }
            }
            else if (block.GetType() == typeof(UIButton))
            {
                UIButton btn = block as UIButton;
                if (btn.BKImage != null && btn.XMLImagePath != null)
                    AddResource(btn.XMLImagePath);
            }
        }

        private void AddResource(string path)
        {
            string name = Path.GetFileNameWithoutExtension(path);
            Resource res = Resources.Find(p => p.Name == name);
            if (res == null)
            {
                res = new Resource();
                Resources.Add(res);
            }
            res.Name = name;
            res.Type = res.GetTypeByExt(Path.GetExtension(path));
            res.Path = "\\Resources\\" + Path.GetFileName(path);
        }

        public override void Save()
        {
            GetViewData();
            base.Save();
        }

        public void DeleteUnit(Unit unit)
        {
            Units.Remove(unit);
            File.Delete(GetFilePath(1, unit.Name));
            if (unit.GetType() == typeof(UIUnit))
            {
                foreach (var page in ((UIUnit)unit).Pages)
                {
                    File.Delete(GetFilePath(3, page.Name));
                }
            }
            Save();
        }

        public void DeletePlan(Plan plan)
        {
            Plans.Remove(plan);
            File.Delete(GetFilePath(2, plan.Name));
            Save();
        }

        public void DeleteRes(Resource res)
        {
            Resources.Remove(res);
            File.Delete(FolderPath + res.Path);
            Save();
        }

        public int GetFileCount()
        {
            int count = Units.Count + Plans.Count;
            Unit unit = Units.Find(p => p.GetType() == typeof(UIUnit));
            if (unit != null)
                count += ((UIUnit)unit).Pages.Count;

            return count;
        }

        public UIUnit GetUIUnit()
        {
            Unit unit = Units.Find(p => p.GetType() == typeof(UIUnit));
            return unit as UIUnit;
        }

        public string CreatePageName()
        {
            string name = "Page";
            UIUnit unit = GetUIUnit();
            for (int i = 0; i < 30; i++)
            {
                if (unit.Pages.Find(p => p.Name == name + i) == null)
                {
                    name += i;
                    break;
                }
            }
            return name;
        }

        public void AddViewPage(ViewPage page)
        {
            UIUnit unit = GetUIUnit();
            unit.Pages.Add(page);
            File.WriteAllText(GetFilePath(3, page.Name), "");
            Save();
        }

        public void DeleteViewPageByName(string name)
        {
            UIUnit unit = GetUIUnit();
            ViewPage page = unit.Pages.Find(p => p.Name == name);
            if (page != null)
            {
                unit.Pages.Remove(page);
                File.Delete(GetFilePath(3, page.Name));
                Save();
            }
        }

        private string GetFilePath(int index, string fileName)
        {
            string path = "";
            switch(index)
            {
                case 1:
                    //单元文件路径
                    path = Path.Combine(FolderPath, "Units", fileName + ".py");
                    break;
                case 2:
                    //计划文件路径
                    path = Path.Combine(FolderPath, "Plans", fileName + ".py");
                    break;
                case 3:
                    //视图数据文件路径
                    path = Path.Combine(FolderPath, "Units\\Pages", fileName + ".xml");
                    break;
                //case 4:
                //    break;
                default:
                    break;
            }
            return path;
        }
    }
}
