namespace SharkSmart
{
    partial class CtlDeviceItem
    {
        /// <summary> 
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
			this.lbName = new System.Windows.Forms.Label();
			this.cbmod = new System.Windows.Forms.ComboBox();
			this.pbar = new System.Windows.Forms.ProgressBar();
			this.btnConfig = new System.Windows.Forms.Button();
			this.btnQucik = new System.Windows.Forms.Button();
			this.btnFolder = new System.Windows.Forms.Button();
			this.cbCheck = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// lbName
			// 
			this.lbName.AutoSize = true;
			this.lbName.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
			this.lbName.ForeColor = System.Drawing.Color.White;
			this.lbName.Location = new System.Drawing.Point(40, 19);
			this.lbName.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.lbName.Name = "lbName";
			this.lbName.Size = new System.Drawing.Size(58, 22);
			this.lbName.TabIndex = 0;
			this.lbName.Text = "label1";
			// 
			// cbmod
			// 
			this.cbmod.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.cbmod.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbmod.ForeColor = System.Drawing.Color.White;
			this.cbmod.FormattingEnabled = true;
			this.cbmod.Location = new System.Drawing.Point(309, 15);
			this.cbmod.Name = "cbmod";
			this.cbmod.Size = new System.Drawing.Size(200, 26);
			this.cbmod.TabIndex = 1;
			this.cbmod.SelectedIndexChanged += new System.EventHandler(this.Cbmod_SelectedIndexChanged);
			// 
			// pbar
			// 
			this.pbar.Location = new System.Drawing.Point(523, 23);
			this.pbar.Name = "pbar";
			this.pbar.Size = new System.Drawing.Size(160, 10);
			this.pbar.TabIndex = 28;
			this.pbar.Visible = false;
			// 
			// btnConfig
			// 
			this.btnConfig.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.btnConfig.BackgroundImage = global::SharkSmart.Properties.Resources.配置;
			this.btnConfig.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
			this.btnConfig.FlatAppearance.BorderColor = System.Drawing.Color.White;
			this.btnConfig.FlatAppearance.BorderSize = 0;
			this.btnConfig.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnConfig.Location = new System.Drawing.Point(688, 5);
			this.btnConfig.Margin = new System.Windows.Forms.Padding(4);
			this.btnConfig.Name = "btnConfig";
			this.btnConfig.Size = new System.Drawing.Size(45, 45);
			this.btnConfig.TabIndex = 29;
			this.btnConfig.UseVisualStyleBackColor = false;
			this.btnConfig.Click += new System.EventHandler(this.BtnConfig_Click);
			// 
			// btnQucik
			// 
			this.btnQucik.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.btnQucik.BackgroundImage = global::SharkSmart.Properties.Resources.play__2_;
			this.btnQucik.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
			this.btnQucik.FlatAppearance.BorderColor = System.Drawing.Color.White;
			this.btnQucik.FlatAppearance.BorderSize = 0;
			this.btnQucik.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnQucik.Location = new System.Drawing.Point(737, 6);
			this.btnQucik.Margin = new System.Windows.Forms.Padding(4);
			this.btnQucik.Name = "btnQucik";
			this.btnQucik.Size = new System.Drawing.Size(45, 45);
			this.btnQucik.TabIndex = 26;
			this.btnQucik.UseVisualStyleBackColor = false;
			this.btnQucik.Click += new System.EventHandler(this.BtnQucik_Click);
			// 
			// btnFolder
			// 
			this.btnFolder.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.btnFolder.BackgroundImage = global::SharkSmart.Properties.Resources.zu;
			this.btnFolder.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
			this.btnFolder.FlatAppearance.BorderColor = System.Drawing.Color.White;
			this.btnFolder.FlatAppearance.BorderSize = 0;
			this.btnFolder.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnFolder.Location = new System.Drawing.Point(786, 5);
			this.btnFolder.Margin = new System.Windows.Forms.Padding(4);
			this.btnFolder.Name = "btnFolder";
			this.btnFolder.Size = new System.Drawing.Size(45, 45);
			this.btnFolder.TabIndex = 30;
			this.btnFolder.UseVisualStyleBackColor = false;
			this.btnFolder.Click += new System.EventHandler(this.BtnFolder_Click);
			// 
			// cbCheck
			// 
			this.cbCheck.AutoSize = true;
			this.cbCheck.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F);
			this.cbCheck.Location = new System.Drawing.Point(18, 24);
			this.cbCheck.Name = "cbCheck";
			this.cbCheck.Size = new System.Drawing.Size(15, 14);
			this.cbCheck.TabIndex = 31;
			this.cbCheck.UseVisualStyleBackColor = true;
			// 
			// CtlDeviceItem
			// 
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.Controls.Add(this.cbCheck);
			this.Controls.Add(this.btnFolder);
			this.Controls.Add(this.btnConfig);
			this.Controls.Add(this.pbar);
			this.Controls.Add(this.btnQucik);
			this.Controls.Add(this.cbmod);
			this.Controls.Add(this.lbName);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.Name = "CtlDeviceItem";
			this.Size = new System.Drawing.Size(858, 60);
			this.Load += new System.EventHandler(this.CtlCompile_Load);
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.CtlCompile_Paint);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbName;
        private System.Windows.Forms.ComboBox cbmod;
        private System.Windows.Forms.Button btnQucik;
        private System.Windows.Forms.ProgressBar pbar;
        private System.Windows.Forms.Button btnConfig;
        private System.Windows.Forms.Button btnFolder;
        public System.Windows.Forms.CheckBox cbCheck;
    }
}
