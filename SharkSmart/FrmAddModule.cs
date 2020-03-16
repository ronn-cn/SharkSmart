using EVClassLib;
using EVTechnology.Common.Controls;
using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmAddModule : EVForm
    {
        public FrmAddModule()
        {
            InitializeComponent();
        }

        private void FrmAddModule_Load(object sender, EventArgs e)
        {
            UpdatePreset();
        }


        private void BtnCreate_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(tbType.Text))
            {
                MessageBox.Show("请输入模块名称!");
                return;
            }
            ExecModule mod = new ExecModule();
            mod.Name = tbType.Text;
            mod.Parent = Engine.Project.ID;
            mod.PresetName = cmbPreset.Text;
            Engine.Project.AddModule(mod);

            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void UpdatePreset()
        {
            this.cmbPreset.Items.Clear();
            List<string> comps = GetComponentLibs();
            foreach (string str in comps)
            {
                cmbPreset.Items.Add(Path.GetFileNameWithoutExtension(str));
            }
        }

        private static List<string> GetComponentLibs()
        {
            List<string> componentLibs = new List<string>();
            string folder = Path.Combine(Environment.CurrentDirectory, SoftwareSetting.SourceCodePath);
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

        private void CmbPreset_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(cmbPreset.Text) && string.IsNullOrEmpty(tbType.Text))
            {
                tbType.Text = cmbPreset.Text;
            }
        }
    }
}
