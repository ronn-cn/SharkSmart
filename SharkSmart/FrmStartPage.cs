using EVClassLib;
using EVTechnology.Common.Controls;
using EVTechnology.Common.Helper;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmStartPage : EVForm
    {
        public FrmStartPage()
        {
            InitializeComponent();
        }

        private Scene Scene;

        private RecordTool tool;

        private string lastlyPath = "";

        private string TmpPath = "";

        private void FrmStartStyle_Load(object sender, EventArgs e)
        {
            SetPaneEnable(plA);

            tool = new RecordTool();
            UpdateProjectList();
            if (!string.IsNullOrEmpty(lastlyPath))
                TmpPath = lastlyPath;
            else
                TmpPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            for (int i = 0; i < 100; i++)
            {
                string path = Path.Combine(TmpPath, "NewProject" + i.ToString());
                if (!Directory.Exists(path))
                {
                    tbProName.Text = "NewProject" + i.ToString();
                    break;
                }
            }

            tbPath.Text = TmpPath + "\\" + tbProName.Text;
            UpdatePreset();
            if (ivComponent.Items.Count > 0)
            {
                ivComponent.Items[0].Selected = true;
                ivComponent.Select();
            }

            Scene = new Scene();
        }

        private void BtnNextOne_Click(object sender, EventArgs e)
        {
            string tip = CheckInfo(false);
            if (!string.IsNullOrEmpty(tip))
            {
                MessageBox.Show(tip);
                return;
            }
            SetPaneEnable(plB);
            UpdatePreview();
        }

        private void BtnNextTwo_Click(object sender, EventArgs e)
        {
            if (ivComponent.Items.Count < 1)
            {
                MessageBox.Show("请至少添加一个模块!");
                return;
            }
            SetPaneEnable(plC);
            UpdatePreview();
        }

        private void BtnPre_Click(object sender, EventArgs e)
        {
            SetPaneEnable(plA);
            UpdatePreview();
        }

        private void BtnPreTwo_Click(object sender, EventArgs e)
        {
            SetPaneEnable(plB);
            UpdatePreview();
        }

        private void UpdateProjectList()
        {
            ivProject.Items.Clear();
            List<HistoryProjectRecord> records = tool.GetProjectRecords();
            if (records.Count > 0)
                lastlyPath = Path.GetDirectoryName(Path.GetDirectoryName(records[0].ProjectPath));

            int prcount = 0;
            foreach (HistoryProjectRecord reord in records)
            {
                if (prcount > 10) break;
                if (File.Exists(reord.ProjectPath))
                {
                    ListViewItem li = new ListViewItem();
                    li.Tag = reord.ProjectID;
                    li.Text = Path.GetFileNameWithoutExtension(reord.ProjectPath);
                    li.ToolTipText = reord.ProjectPath;
                    li.ImageIndex = 0;
                    ivProject.Items.Add(li);
                    prcount++;
                }
                else
                {
                    tool.DeleteProjectRecord(reord.ProjectID, false);
                }
            }
        }

        private void SetPaneEnable(Panel pl)
        {
            Panel[] pls = new Panel[3] { plA, plB, plC };
            foreach (var p in pls)
            {
                if (p == pl)
                {
                    SetPaneEnable(p, true);
                }
                else
                {
                    SetPaneEnable(p, false);
                }
            }
        }

        private void SetPaneEnable(Panel pnl, bool enable)
        {
            switch (pnl.Name)
            {
                case "plA":
                    if (enable)
                    {
                        pnl.Enabled = true;
                        pnl.BackColor = Color.FromArgb(32, 45, 89);
                        tbProName.BackColor = Color.FromArgb(43, 59, 110);
                        tbPath.BackColor = Color.FromArgb(43, 59, 110);
                        btnOpen.BackColor = Color.FromArgb(64, 80, 141);
                        btnNextOne.BackColor = Color.FromArgb(118, 164, 92);
                    }
                    else
                    {
                        pnl.BackColor = Color.FromArgb(53, 62, 95);
                        tbProName.BackColor = Color.FromArgb(58, 69, 105);
                        tbPath.BackColor = Color.FromArgb(58, 69, 105);
                        btnOpen.BackColor = Color.FromArgb(58, 69, 105);
                        btnNextOne.BackColor = Color.FromArgb(95, 121, 97);
                        pnl.Enabled = false;
                    }
                    break;
                case "plB":
                    if (enable)
                    {
                        pnl.Enabled = true;
                        pnl.BackColor = Color.FromArgb(32, 45, 89);
                        btnPre.BackColor = Color.FromArgb(64, 80, 141);
                        btnAdd.BackColor = Color.FromArgb(64, 80, 141);
                        cmbPreset.BackColor = Color.FromArgb(43, 59, 110);
                        tbName.BackColor = Color.FromArgb(43, 59, 110);
                        ivComponent.BackColor = Color.FromArgb(43, 59, 110);
                        btnDelete.BackColor = Color.FromArgb(177, 68, 65);
                        btnNextTwo.BackColor = Color.FromArgb(118, 164, 92);
                    }
                    else
                    {
                        pnl.BackColor = Color.FromArgb(53, 62, 95);
                        btnPre.BackColor = Color.FromArgb(58, 69, 105);
                        cmbPreset.BackColor = Color.FromArgb(58, 69, 105);
                        tbName.BackColor = Color.FromArgb(58, 69, 105);
                        ivComponent.BackColor = Color.FromArgb(58, 69, 105);
                        btnDelete.BackColor = Color.FromArgb(155, 87, 101);
                        btnNextTwo.BackColor = Color.FromArgb(95, 121, 97);
                        pnl.Enabled = false;
                    }
                    break;
                case "plC":
                    if (enable)
                    {
                        pnl.Enabled = true;
                        pnl.BackColor = Color.FromArgb(32, 45, 89);
                        btnPreTwo.BackColor = Color.FromArgb(64, 80, 141);
                        tvPreview.BackColor = Color.FromArgb(43, 59, 110);
                        btnCreate.BackColor = Color.FromArgb(118, 164, 92);
                    }
                    else
                    {
                        pnl.BackColor = Color.FromArgb(53, 62, 95);
                        btnPreTwo.BackColor = Color.FromArgb(58, 69, 105);
                        tvPreview.BackColor = Color.FromArgb(58, 69, 105);
                        btnCreate.BackColor = Color.FromArgb(95, 121, 97);
                        pnl.Enabled = false;
                    }
                    break;
            }
        }

        private void BtnCreate_Click(object sender, EventArgs e)
        {
            string tip = CheckInfo(true);
            if (!string.IsNullOrEmpty(tip))
            {
                MessageBox.Show(tip);
                return;
            }

            Scene.Name = tbProName.Text;
            Scene.WorkPath = tbPath.Text;
            Engine.Project = Scene;

            foreach (var mod in tmpModules)
            {
                Scene.AddModule(mod);
            }

            Scene.Build();

            HistoryProjectRecord rec = new HistoryProjectRecord();
            rec.ProjectID = Scene.ID;
            rec.ProjectPath = Path.Combine(Scene.WorkPath, Scene.Name + ".sst");
            rec.Time = DateTime.Now;
            tool.AddProjectRecord(rec);

            this.Hide();
            FrmMain frm = new FrmMain();
            frm.StartPosition = FormStartPosition.CenterParent;
            frm.Show();
        }

        private string CheckInfo(bool allInfo)
        {
            if (string.IsNullOrEmpty(tbProName.Text))
                return "请输入场景名称!";
            else if (string.IsNullOrEmpty(tbPath.Text))
                return "请输入路径!";
            else if (Directory.Exists(tbPath.Text))
                return "该路径已存在!";
            else if (!Directory.Exists(Path.GetDirectoryName(tbPath.Text)))
                return "该路径不存在!";
            else if (allInfo && tmpModules.Count < 1)
                return "请至少选择一个预设";
            else
                return "";
        }

        private List<Module> tmpModules = new List<Module>();

        private void BtnAdd_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(cmbPreset.Text))
                return;

            if (ivComponent.Items.Find(tbName.Text, false).Count() > 0)
            {
                MessageBox.Show("该名称已存在!");
                return;
            }
            ExecModule mod = new ExecModule();
            mod.Name = tbName.Text;
            mod.PresetName = cmbPreset.SelectedItem.ToString();
            mod.Parent = Scene.ID;
            tmpModules.Add(mod);

            ListViewItem item = new ListViewItem
            {
                Name = tbName.Text,
                Tag = tbName.Text,
                Text = tbName.Text,
                ImageIndex = 0
            };
            ivComponent.Items.Add(item);
        }

        private void BtnDelete_Click(object sender, EventArgs e)
        {
            if (ivComponent.SelectedItems.Count < 1)
                return;
            Module mod = Scene.GetModule(ivComponent.SelectedItems[0].Text);
            if (mod != null)
                Scene.DeleteModule(mod);
            ivComponent.Items.Remove(ivComponent.SelectedItems[0]);
        }

        private void BtnBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = TmpPath;
            dlg.Filter = "场景|*.sst";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                OpenProject(dlg.FileName);
            }
        }

        private void OpenProject(string path)
        {
            Scene sp = FileHelper.AnalysisSerializeFile<Scene>(path, FileType.XMLFile);
            sp.Name = Path.GetFileNameWithoutExtension(path);
            sp.WorkPath = Path.GetDirectoryName(path);
            Engine.Project = sp;
            HistoryProjectRecord record = new HistoryProjectRecord
            {
                ProjectID = sp.ID,
                ProjectPath = sp.WorkPath + "\\" + sp.Name + ".sst",
                Time = DateTime.Now
            };
            tool.UpdateProjectRecord(record);
            foreach (var mod in sp.ModuleDatas)
            {
                mod.WorkPath = sp.WorkPath + "\\" + mod.Name + "\\" + mod.Name + ".mt";
            }
            this.Hide();
            FrmMain frm = new FrmMain();
            frm.Show();
        }

        private void UpdatePreset()
        {
            cmbPreset.Items.Clear();
            List<string> comps = GetComponentLibs();
            foreach (string str in comps)
            {
                cmbPreset.Items.Add(Path.GetFileNameWithoutExtension(str));
            }
        }

        private static List<string> GetComponentLibs()
        {
            List<string> componentLibs = new List<string>();
            string folder = Path.Combine(Environment.CurrentDirectory, "Library\\Components");
            DirectoryInfo directory = new DirectoryInfo(folder);
            DirectoryInfo[] infos = directory.GetDirectories();
            foreach (DirectoryInfo info in infos)
            {
                string file = Path.Combine(info.FullName, info.Name + ".lib");
                if (File.Exists(file))
                {
                    componentLibs.Add(file);
                }
            }
            return componentLibs;
        }

        private void BtnOpen_Click(object sender, EventArgs e)
        {
            string pa = FormHelper.GetBrowserPath();
            if (!string.IsNullOrEmpty(pa))
                TmpPath = pa;
            tbPath.Text = TmpPath + "\\" + tbProName.Text;
        }

        private void CmbPreset_SelectedIndexChanged(object sender, EventArgs e)
        {
            tbName.Text = cmbPreset.SelectedItem.ToString();
        }

        private void TbProName_TextChanged(object sender, EventArgs e)
        {
            tbPath.Text = TmpPath + "\\" + tbProName.Text;
        }

        private void IvProject_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (ivProject.SelectedItems.Count > 0)
            {
                string path = ivProject.SelectedItems[0].ToolTipText;
                if (!File.Exists(path))
                {
                    MessageBox.Show("文件已删除!");
                    Guid spId = (Guid)ivProject.SelectedItems[0].Tag;
                    tool.DeleteProjectRecord(spId, false);
                    ivProject.Items.RemoveAt(ivProject.SelectedIndices[0]);
                    return;
                }
                OpenProject(path);
            }
        }

        private void UpdatePreview()
        {
            tvPreview.Items.Clear();
            if (string.IsNullOrEmpty(tbProName.Text))
                return;

            EVTreeItem root = new EVTreeItem();
            root.IsExpanded = true;
            root.Text = tbProName.Text;
            tvPreview.Items.Add(root);

            if (Scene == null || tmpModules.Count < 0)
                return;

            foreach (var mod in tmpModules)
            {
                EVTreeItem node = new EVTreeItem();
                node.Text = mod.Name;
                root.Items.Add(node);

                EVTreeItem clda = new EVTreeItem();
                clda.Text = "Plans";
                node.Items.Add(clda);

                EVTreeItem cldb = new EVTreeItem();
                cldb.Text = "Units";
                node.Items.Add(cldb);

                EVTreeItem cldc = new EVTreeItem();
                cldc.Text = "Resources";
                node.Items.Add(cldc);
            }
            tvPreview.ExpandAll();
            tvPreview.Refresh();
        }

        private void IvProject_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (ivProject.SelectedItems.Count > 0)
                {
                    string path = ivProject.SelectedItems[0].ToolTipText;
                    if (!File.Exists(path))
                    {
                        MessageBox.Show("文件已删除!");
                        Guid spId = (Guid)ivProject.SelectedItems[0].Tag;
                        tool.DeleteProjectRecord(spId, false);
                        ivProject.Items.RemoveAt(ivProject.SelectedIndices[0]);
                        return;
                    }
                    OpenProject(path);
                }
            }
        }

        private void IvProject_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                cmsMenu.Show(ivProject, e.Location);
            }
        }

        private void TsmDeleteRecord_Click(object sender, EventArgs e)
        {
            DeleteProject(false);
        }

        private void TsmDeleteProject_Click(object sender, EventArgs e)
        {
            DeleteProject(true);
        }

        private void DeleteProject(bool IsDeleteFile)
        {
            if (ivProject.SelectedItems.Count < 1)
                return;

            tool.DeleteProjectRecord((Guid)ivProject.SelectedItems[0].Tag, IsDeleteFile);
            UpdateProjectList();
            //ivProject.Items.RemoveAt(ivProject.SelectedIndices[0]);
        }

        private void LlQuickCreate_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Scene.Name = tbProName.Text;
            Scene.WorkPath = tbPath.Text;
            Engine.Project = Scene;

            foreach (var mod in tmpModules)
            {
                Scene.AddModule(mod);
            }

            Scene.Build();

            HistoryProjectRecord rec = new HistoryProjectRecord();
            rec.ProjectID = Scene.ID;
            rec.ProjectPath = Path.Combine(Scene.WorkPath, Scene.Name + ".sst");
            rec.Time = DateTime.Now;
            tool.AddProjectRecord(rec);

            this.Hide();
            FrmMain frm = new FrmMain();
            frm.StartPosition = FormStartPosition.CenterParent;
            frm.Show();
        }

        private void button1_Click(object sender, EventArgs e)
        {
        }
    }

}
