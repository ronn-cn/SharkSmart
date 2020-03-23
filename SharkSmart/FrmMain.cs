using EVClassLib;
using EVTechnology.Controls;
using EVTechnology.Logging;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmMain : EVForm
    {
        private FrmEditor cacheEditor;

        public FrmMain()
        {
            InitializeComponent();
            Rectangle rec = Screen.PrimaryScreen.Bounds;
            if (rec.Width > 1920 && rec.Height > 1080)
                Size = new Size(1920, 1080);
            else
                WindowState = FormWindowState.Maximized;
        }

        private void FrmMain_Load(object sender, EventArgs e)
        {
            tbMain.Show();
            InitSystem();

            if (Engine.Project != null)
                this.Text = Engine.Project.Name + " - SharkSmart";

            UpdateExplorer();
            tbMain.Items.Clear();
            CreateCodePage();
        }

        private void FrmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void InitSystem()
        {
            Engine.Setting = new SoftwareSetting();

            Engine.Logger = new Logger
            {
                OutputLevel = Level.FATAL,
                IsWriteFile = Engine.Setting.IsUseDebugLog,
                WriteFilePath = Engine.Setting.OutputLogPath,
                WriteFileTimeSpan = Engine.Setting.AutoSaveLogFileTimeSpan
            };
        }

        private void UpdateExplorer()
        {
            if (Engine.Project == null)
                return;

            List<TreeNode> nde = new List<TreeNode>();

            tvExplorer.Nodes.Clear();
            TreeNode root = CreateNode(null, Engine.Project.Name, ImageType.Folder);

            foreach (var data in Engine.Project.ModuleDatas)
            {
                Module mod = Engine.Project.GetModule(data.ID);
                TreeNode modNode = CreateNode(root, mod.Name, ImageType.Folder);
                modNode.Tag = modNode.Text;

                if (mod.GetType() != typeof(ExecModule)) continue;

                TreeNode nodeD = CreateNode(modNode, "Datas", ImageType.Table);
                nodeD.Tag = modNode.Text + "\\Datas";

                TreeNode nodeP = CreateNode(modNode, "Plans", ImageType.Folder);
                nodeP.Tag = modNode.Text + "\\Plans";
                TreeNode nodeU = CreateNode(modNode, "Units", ImageType.Folder);
                nodeU.Tag = modNode.Text + "\\Units";
                TreeNode nodeR = CreateNode(modNode, "Resources", ImageType.Folder);
                nodeR.Tag = modNode.Text + "\\Resources";

                TreeNode nodeV = CreateNode(nodeU, "Pages", ImageType.Folder);
                nodeV.Tag = modNode.Text + "\\Units\\Pages";
                foreach (var unit in ((ExecModule)mod).Units)
                {

                    TreeNode nodeu = CreateNode(nodeU, unit.Name, ImageType.Code);
                    nodeu.Tag = Path.Combine(Engine.Project.WorkPath, modNode.Text, nodeU.Text, nodeu.Text + ".py");

                    if (unit.GetType() != typeof(UIUnit)) continue;
                    foreach (var page in ((UIUnit)unit).Pages)
                    {
                        TreeNode nodeui = CreateNode(nodeV, page.Name, ImageType.Image);
                        nodeui.Tag = Path.Combine(Engine.Project.WorkPath, modNode.Text, nodeU.Text, "Pages", nodeui.Text + ".xml");
                        for (int i = 1; i < page.Controls.Count; i++)
                        {
                            CreateNode(nodeui, page.Controls[i].Name.Replace(page.Name + "_", ""), ImageType.Control);
                        }
                        nde.Add(nodeui);
                    }
                }

                foreach (var plan in ((ExecModule)mod).Plans)
                {
                    TreeNode nodep = CreateNode(nodeP, plan.Name, ImageType.Code);
                    nodep.Tag = Path.Combine(Engine.Project.WorkPath, modNode.Text, nodeP.Text, nodep.Text + ".py");
                }
            }

            tvExplorer.ExpandAll();
            foreach (var no in nde)
                no.Collapse();
        }

        private enum ImageType
        {
            Folder,
            Code,
            Image,
            Table,
            Control,
        }

        //创建文件夹节点
        private TreeNode CreateNode(TreeNode root, string text, ImageType type)
        {
            TreeNode node = new TreeNode(text)
            {
                ImageIndex = (int)type,
                SelectedImageIndex = (int)type
            };
            if (root == null)
                tvExplorer.Nodes.Add(node);
            else
            {
                root.Nodes.Add(node);
            }
            return node;
        }

        private void TvExplorer_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (tvExplorer.SelectedNode == null || tvExplorer.SelectedNode.Tag == null) return;

            string path = tvExplorer.SelectedNode.Tag.ToString();
            string ext = Path.GetExtension(path);
            EVTabPage page = this.tbMain.Items.Find(p => p.TabPath == path);
            if (page != null)
            {
                tbMain.SelectedItem = page;
            }
            else
            {
                page = new EVTabPage();
                page.TabText = Path.GetFileNameWithoutExtension(path);
                page.TabPath = path;

                if (ext == ".py")
                    OpenCodeView(page, path);
                else if (ext == ".xml")
                    OpenViewPage(page, path);
                else if (Path.GetFileName(path).Equals("Datas"))
                    OpenDatasView(page);
                else if (Path.GetFileName(path).Equals("Resources"))
                    OpenResView(page);
                else
                    return;
            }
        }

        public void SkipCodeView(string path)
        {
            EVTabPage page = this.tbMain.Items.Find(p => p.TabPath == path);
            if (page != null)
            {
                //需要重新加载
                FrmEditor frm = page.Content.Controls[0] as FrmEditor;
                tbMain.SelectedItem = page;
                frm.UpdateText();
            }
            else
            {
                page = new EVTabPage();
                page.TabText = Path.GetFileNameWithoutExtension(path);
                page.TabPath = path;
                OpenCodeView(page, path);
            }
        }

        /// <summary>
        /// 打开代码页
        /// </summary>
        private void OpenCodeView(EVTabPage page, string path)
        {
            FrmEditor editor = cacheEditor;
            editor.FilePath = path;
            AddPageToTB(editor, page);
            CreateCodePage();
        }

        /// <summary>
        /// 打开数据页
        /// </summary>
        private void OpenDatasView(EVTabPage page)
        {
            Module mod = GetModuleByNode();
            if (mod == null) return;
            FrmData frm = new FrmData();
            frm.mod = (ExecModule)mod;
            AddPageToTB(frm, page);
        }

        /// <summary>
        /// 打开视图页
        /// </summary>
        private void OpenViewPage(EVTabPage page, string path)
        {
            FrmViewPage frm = new FrmViewPage();
            Size tmp = new Size(800, 480);
            Module mod = GetModuleByNode();
            frm.FilePath = path;
            frm.Mod = mod;
            frm.Main = this;
            if (string.IsNullOrEmpty(File.ReadAllText(path)))
            {
                if (mod != null)
                {
                    Unit unit = ((ExecModule)mod).Units.Find(p => p.GetType() == typeof(UIUnit));
                    if (unit != null)
                    {
                        tmp = ((UIUnit)unit).DPI;
                    }
                    frm.tmp = tmp;
                }
            }
            AddPageToTB(frm, page);
        }

        private void OpenResView(EVTabPage page)
        {
            Module mod = GetModuleByNode();
            if (mod == null) return;
            FrmResources frm = new FrmResources();
            frm.Mod = (ExecModule)mod;
            AddPageToTB(frm, page);
        }

        private void AddPageToTB(Control ctl, EVTabPage page)
        {
            if (ctl is Form)
            {
                ((Form)ctl).FormBorderStyle = FormBorderStyle.None;
                ((Form)ctl).TopLevel = false;
                ctl.Show();
            }
            ctl.Dock = DockStyle.Fill;
            page.Content.Controls.Add(ctl);
            tbMain.Items.Add(page);
            tbMain.SelectedItem = page;
        }

        private void CreateCodePage()
        {
            cacheEditor = new FrmEditor();
            cacheEditor.FormBorderStyle = FormBorderStyle.None;
            cacheEditor.TopLevel = false;
            cacheEditor.Dock = DockStyle.Fill;
            cacheEditor.Size = Size;
        }

        private void BtnCatalog_Click(object sender, EventArgs e)
        {
            //打开项目文件夹
            Process.Start(Engine.Project.WorkPath);
        }

        private void TsbRefresh_Click(object sender, EventArgs e)
        {
            UpdateExplorer();
        }

        private CtlDebug pageDebug;

        private void BtnDebug_Click(object sender, EventArgs e)
        {
            OpenDebugPage();
        }

        /// <summary>
        /// 打开调试面板
        /// </summary>
        private void OpenDebugPage()
        {
            if (pageDebug == null)
            {
                pageDebug = new CtlDebug();
                pageDebug.Dock = DockStyle.Fill;
                EVTabPage page = new EVTabPage();
                page.TabText = "调试管理器";
                page.Content = pageDebug;
                tbMain.Items.Add(page);
                tbMain.SelectedItem = page;
            }
            else
            {
                EVTabPage page = this.tbMain.Items.Find(p => p.TabText == "调试管理器");
                if (page != null)
                {
                    this.tbMain.SelectedItem = page;
                }
                else
                {
                    page = new EVTabPage
                    {
                        TabText = "调试管理器",
                        Content = pageDebug
                    };
                    tbMain.Items.Add(page);
                    tbMain.SelectedItem = page;
                }
            }
        }

        private void BtnProject_Click(object sender, EventArgs e)
        {
            //添加新模块
            FrmAddModule frm = new FrmAddModule();
            if (frm.ShowDialog() == DialogResult.OK)
                UpdateExplorer();
        }

        private void BtnSave_Click(object sender, EventArgs e)
        {
            Engine.Project.Save();
        }

        private void TsmAddPlan_Click(object sender, EventArgs e)
        {
            ExecModule mod = (ExecModule)GetModuleByNode();
            if (mod == null) return;
            //创建计划
            FrmAddPlan frm = new FrmAddPlan();
            frm.Tag = "Plan" + mod.Plans.Count;
            frm.Mod = mod;
            if (frm.ShowDialog() == DialogResult.OK)
            {
                mod.AddPlan(frm.Plan);
                UpdateExplorer();
            }
        }

        private void TsmAddUnit_Click(object sender, EventArgs e)
        {
            ExecModule mod = (ExecModule)GetModuleByNode();
            if (mod == null) return;
            FrmAddUnit frm = new FrmAddUnit();
            frm.Tag = "Unit" + mod.Units.Count;
            if (frm.ShowDialog() == DialogResult.OK)
            {
                mod.AddUnit(frm.Unit);
                UpdateExplorer();
            }
        }

        private void TsmDelete_Click(object sender, EventArgs e)
        {
            ExecModule mod = (ExecModule)GetModuleByNode();
            if (mod == null) return;

            if (tvExplorer.SelectedNode.Parent.Text == "Plans")
            {
                Plan plan = mod.Plans.Find(p => p.Name == tvExplorer.SelectedNode.Text);
                if (plan != null)
                {
                    mod.DeletePlan(plan);
                    mod.Save();
                }
            }
            else if (tvExplorer.SelectedNode.Parent.Text == "Units")
            {
                Unit unit = mod.Units.Find(p => p.Name == tvExplorer.SelectedNode.Text);
                if (unit != null)
                {
                    mod.DeleteUnit(unit);
                    mod.Save();
                }
            }
            else if (tvExplorer.SelectedNode.Parent.Text == "Pages")
            {
                mod.DeleteViewPageByName(tvExplorer.SelectedNode.Text);
            }
            UpdateExplorer();
        }

        private void TsmOpen_Click(object sender, EventArgs e)
        {
            if (tvExplorer.SelectedNode == null || tvExplorer.SelectedNode.Tag == null)
                return;
            if (tvExplorer.SelectedNode.ImageIndex == (int)ImageType.Folder)
            {
                //打开文件夹  
                string path = Path.Combine(Engine.Project.WorkPath, tvExplorer.SelectedNode.Tag.ToString());
                Process.Start(path);
            }
            else
                TvExplorer_NodeMouseDoubleClick(sender, null);
        }

        private void TsmRename_Click(object sender, EventArgs e)
        {

        }

        private void TvExplorer_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            //右键触发事件
            if (e.Button == MouseButtons.Right)
            {
                tvExplorer.SelectedNode = e.Node;
                if (e.Node.Level == 0) return;      //右键根节点 返回
                cmsTree.Show(tvExplorer, e.Location);

                tsmDelete.Enabled = false;
                tsmOpen.Enabled = false;
                tsmRename.Enabled = false;
                tsmAddView.Enabled = false;
                tsmDeleteMod.Enabled = false;

                if (tvExplorer.SelectedNode.ImageIndex == (int)ImageType.Folder)
                {
                    tsmOpen.Enabled = true;
                    if (e.Node.Level == 1)
                    {
                        tsmDeleteMod.Enabled = true;
                    }
                    else if (e.Node.Level == 3)
                    {
                        ExecModule tmpMod = (ExecModule)GetModuleByNode();
                        if (tmpMod == null || e.Node.Parent.Text == "Plans") return;
                        if (e.Node.Text == "Pages" && tmpMod.Units.Find(p => p.GetType() == typeof(UIUnit)) != null)
                            tsmAddView.Enabled = true;
                    }
                }
                else if (e.Node.Level == 3 || e.Node.Level == 4)
                {
                    tsmDelete.Enabled = true;
                    tsmOpen.Enabled = true;
                    tsmRename.Enabled = true;
                }
            }
        }

        private Module GetModuleByNode()
        {
            TreeNode node = tvExplorer.SelectedNode;
            if (node == null) return null;
            Module mod = Engine.Project.GetModule(GetNodeTextByLevel(1, node));
            return mod;
        }

        private string GetNodeTextByLevel(int level, TreeNode node)
        {
            string text = "";
            if (node.Level < level) return text;
            else if (node.Level == level) return node.Text;
            else
                text = GetNodeTextByLevel(level, node.Parent);
            return text;
        }

        private void TsmAddView_Click(object sender, EventArgs e)
        {
            ExecModule tmpMod = (ExecModule)GetModuleByNode();
            FrmAddView frm = new FrmAddView();
            frm.Tag = tmpMod.CreatePageName();
            if (frm.ShowDialog() == DialogResult.OK)
            {
                tmpMod.AddViewPage(frm.Page);
                UpdateExplorer();
            }
        }

        private void TsmDeleteMod_Click(object sender, EventArgs e)
        {
            Module mod = GetModuleByNode();
            mod.Delete();
            Engine.Project.DeleteModule(mod);
            UpdateExplorer();
        }

        private void BtnNewBulid_Click(object sender, EventArgs e)
        {
            Process process = new Process();//创建进程对象    
            ProcessStartInfo startInfo = new ProcessStartInfo("SharkSmart.exe"); // 括号里是(程序名,参数)
            process.StartInfo = startInfo;
            process.Start();
        }

        private void BtnAbout_Click(object sender, EventArgs e)
        {
            FrmAbout frm = new FrmAbout();
            frm.ShowDialog();
        }
    }
}
