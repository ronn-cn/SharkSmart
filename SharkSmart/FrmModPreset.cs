using EVClassLib;
using EVTechnology.Common.Helper;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmModPreset : Form
    {
        public FrmModPreset()
        {
            InitializeComponent();
        }

        private ExecModPreset mod;

        private void BtnSave_Click(object sender, EventArgs e)
        {
            mod.Name = textBox1.Text;
            mod.Description = textBox2.Text;
            mod.IsDisplay = checkBox1.Checked;
            FileHelper.CreateSerializeFile("preset/" + mod.Name + ".lib", mod, FileType.BinaryFile);
        }

        private void BtnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void FrmModPreset_Load(object sender, EventArgs e)
        {
            mod = new ExecModPreset();
        }
    }

    [Serializable]
    public class ExecModPreset
    {
        public string Name { set; get; }

        public string Description { set; get; }

        public bool IsDisplay { set; get; }
    }
}
