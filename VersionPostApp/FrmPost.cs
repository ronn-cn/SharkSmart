using EVTechnology.Helper;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace VersionPostApp
{
    public partial class FrmPost : Form
    {
        public FrmPost()
        {
            InitializeComponent();
        }

        List<string> paths = new List<string>();

        private void BtnSubmit_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(tbAdress.Text)) return;

            HttpPostFileRequestClient httpPost = new HttpPostFileRequestClient();
            httpPost.SetField("app_version", tbVersion.Text);
            httpPost.SetField("version_type", (cbType.SelectedIndex+1).ToString());

            FileStream fs;
            foreach (var path in paths)
            {
                fs = new FileStream(path, FileMode.Open, FileAccess.Read);
                httpPost.SetField("appfiles[]", path, "application/octet-stream", fs);
            }

            HttpWebResponse webRespon;
            webRespon = httpPost.Post(tbAdress.Text);
            Stream s = webRespon.GetResponseStream();
            //读取服务器端返回的消息  
            StreamReader sr = new StreamReader(s);
            String sReturnString = sr.ReadLine();
        }

        private void BtnReset_Click(object sender, EventArgs e)
        {
            tbAdress.Text = "http://172.16.7.11:8000/uploadFile.php";
            tbAdress.Text = "";
            tbVersion.Text = "";
            cbType.SelectedIndex = 0;
            paths.Clear();
        }

        private void BtnUpload_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Multiselect = true;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                if (dlg.FileNames.Length > 0)
                {
                    foreach (var name in dlg.FileNames)
                    {
                        if (paths.Contains(name))
                            continue;
                        
                        paths.Add(name);
                    }
                }
            }
            UpdateListBox();
        }

        private void UpdateListBox()
        {
            listBox2.Items.Clear();
            foreach (var path in paths)
            {
                listBox2.Items.Add(Path.GetFileName(path)); 
            }
        }

        private void InitCombox()
        {
            cbType.DropDownStyle = ComboBoxStyle.DropDownList;
            cbType.Items.Clear();
            cbType.Items.Add("公测版");
            cbType.Items.Add("定制版");
            cbType.Items.Add("测试版(公开)");
            cbType.Items.Add("测试版(私有)");
        }

        private void FrmPost_Load(object sender, EventArgs e)
        {
            InitCombox();
            tbAdress.Text = "http://172.16.7.11:8000/uploadFile.php";
            tbVersion.Text = "11111111";
        }
    }
}
