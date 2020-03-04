using EVClassLib;
using EVTechnology.Common.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmAddResources : EVForm
    {

        public Resource Res { set; get; }

        public FrmAddResources()
        {
            InitializeComponent();
        }

        private void FrmAddView_Load(object sender, EventArgs e)
        {
            Res = new Resource();
        }

        private void BtnCreate_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(tbName.Text))
            {
                MessageBox.Show("请输入名称!");
                return;
            }
            else if (string.IsNullOrEmpty(textBox1.Text))
            {
                MessageBox.Show("请选择文件!");
                return;
            }
            else if (!File.Exists(textBox1.Text))
            {
                MessageBox.Show("文件路径错误!");
                return;
            }
            Res.Name = tbName.Text;
            Res.Type = Res.GetTypeByExt(Path.GetExtension(textBox1.Text));
            Res.Path = textBox1.Text;
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void BtnOpen_Click(object sender, EventArgs e)
        {
            //添加资源
            OpenFileDialog open = new OpenFileDialog();
            open.Filter = "(图片)|*.jpg;*.png;*bmp|(音频)|*.mp3;*.wav;*.wma";
            if (open.ShowDialog() == DialogResult.OK)
            {
                textBox1.Text = open.FileName;
            }
        }
    }
}
