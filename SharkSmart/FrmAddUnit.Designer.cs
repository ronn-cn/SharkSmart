namespace SharkSmart
{
    partial class FrmAddUnit
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            EVTechnology.Controls.ItemColors itemColors1 = new EVTechnology.Controls.ItemColors();
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("空白单元");
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem("视图单元");
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.ivProject = new EVTechnology.Controls.EVIconView();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnCreate = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.nudHight = new System.Windows.Forms.NumericUpDown();
            this.nudWidth = new System.Windows.Forms.NumericUpDown();
            this.nudLight = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.rb3 = new System.Windows.Forms.RadioButton();
            this.rb1 = new System.Windows.Forms.RadioButton();
            this.rb2 = new System.Windows.Forms.RadioButton();
            this.rb0 = new System.Windows.Forms.RadioButton();
            this.tbName = new System.Windows.Forms.TextBox();
            this.panel3 = new System.Windows.Forms.Panel();
            this.TitleBar.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudHight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudLight)).BeginInit();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // TitleBar
            // 
            this.TitleBar.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.TitleBar.Controls.Add(this.label3);
            this.TitleBar.Location = new System.Drawing.Point(4, 4);
            this.TitleBar.Size = new System.Drawing.Size(592, 60);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(37)))), ((int)(((byte)(56)))), ((int)(((byte)(89)))));
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.ivProject);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel1.Location = new System.Drawing.Point(4, 64);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(200, 432);
            this.panel1.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(63, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 20);
            this.label1.TabIndex = 19;
            this.label1.Text = "单元模板";
            // 
            // ivProject
            // 
            this.ivProject.AutoArrange = false;
            this.ivProject.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(37)))), ((int)(((byte)(56)))), ((int)(((byte)(89)))));
            this.ivProject.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ivProject.Cursor = System.Windows.Forms.Cursors.Default;
            this.ivProject.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.ivProject.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ivProject.ForeColor = System.Drawing.Color.Black;
            this.ivProject.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.ivProject.HideSelection = false;
            itemColors1.ItemActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(231)))), ((int)(((byte)(234)))), ((int)(((byte)(246)))));
            itemColors1.ItemBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(107)))), ((int)(((byte)(119)))), ((int)(((byte)(141)))));
            itemColors1.ItemHoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
            this.ivProject.ItemColors = itemColors1;
            this.ivProject.ItemRadius = 5;
            this.ivProject.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2});
            this.ivProject.ItemSize = new System.Drawing.Size(200, 50);
            this.ivProject.LabelWrap = false;
            this.ivProject.Location = new System.Drawing.Point(0, 34);
            this.ivProject.MultiSelect = false;
            this.ivProject.Name = "ivProject";
            this.ivProject.OwnerDraw = true;
            this.ivProject.ShowGroups = false;
            this.ivProject.ShowItemToolTips = true;
            this.ivProject.Size = new System.Drawing.Size(200, 398);
            this.ivProject.TabIndex = 18;
            this.ivProject.TextAlign = System.Drawing.ContentAlignment.TopLeft;
            this.ivProject.TileSize = new System.Drawing.Size(200, 50);
            this.ivProject.UseCompatibleStateImageBehavior = false;
            this.ivProject.View = System.Windows.Forms.View.Tile;
            this.ivProject.SelectedIndexChanged += new System.EventHandler(this.IvProject_SelectedIndexChanged);
            // 
            // btnCancel
            // 
            this.btnCancel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(177)))), ((int)(((byte)(68)))), ((int)(((byte)(65)))));
            this.btnCancel.FlatAppearance.BorderSize = 0;
            this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCancel.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.ForeColor = System.Drawing.Color.White;
            this.btnCancel.Location = new System.Drawing.Point(417, 436);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(150, 36);
            this.btnCancel.TabIndex = 15;
            this.btnCancel.Text = "取  消";
            this.btnCancel.UseVisualStyleBackColor = false;
            this.btnCancel.Click += new System.EventHandler(this.BtnCancel_Click);
            // 
            // btnCreate
            // 
            this.btnCreate.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(118)))), ((int)(((byte)(164)))), ((int)(((byte)(92)))));
            this.btnCreate.FlatAppearance.BorderSize = 0;
            this.btnCreate.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCreate.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCreate.ForeColor = System.Drawing.Color.White;
            this.btnCreate.Location = new System.Drawing.Point(233, 436);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(150, 36);
            this.btnCreate.TabIndex = 14;
            this.btnCreate.Text = "创  建";
            this.btnCreate.UseVisualStyleBackColor = false;
            this.btnCreate.Click += new System.EventHandler(this.BtnCreate_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(243, 96);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 20);
            this.label2.TabIndex = 10;
            this.label2.Text = "单元名称:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(63, 22);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 20);
            this.label3.TabIndex = 11;
            this.label3.Text = "新建单元";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
            this.label6.ForeColor = System.Drawing.Color.White;
            this.label6.Location = new System.Drawing.Point(183, 184);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(14, 18);
            this.label6.TabIndex = 34;
            this.label6.Text = "*";
            // 
            // nudHight
            // 
            this.nudHight.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
            this.nudHight.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.nudHight.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F);
            this.nudHight.ForeColor = System.Drawing.Color.White;
            this.nudHight.Location = new System.Drawing.Point(205, 182);
            this.nudHight.Maximum = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            this.nudHight.Name = "nudHight";
            this.nudHight.Size = new System.Drawing.Size(85, 26);
            this.nudHight.TabIndex = 33;
            this.nudHight.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.nudHight.Value = new decimal(new int[] {
            480,
            0,
            0,
            0});
            // 
            // nudWidth
            // 
            this.nudWidth.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
            this.nudWidth.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.nudWidth.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F);
            this.nudWidth.ForeColor = System.Drawing.Color.White;
            this.nudWidth.Location = new System.Drawing.Point(90, 182);
            this.nudWidth.Maximum = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            this.nudWidth.Name = "nudWidth";
            this.nudWidth.Size = new System.Drawing.Size(85, 26);
            this.nudWidth.TabIndex = 32;
            this.nudWidth.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.nudWidth.Value = new decimal(new int[] {
            800,
            0,
            0,
            0});
            // 
            // nudLight
            // 
            this.nudLight.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
            this.nudLight.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.nudLight.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F);
            this.nudLight.ForeColor = System.Drawing.Color.White;
            this.nudLight.Location = new System.Drawing.Point(90, 227);
            this.nudLight.Name = "nudLight";
            this.nudLight.Size = new System.Drawing.Size(200, 26);
            this.nudLight.TabIndex = 31;
            this.nudLight.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.nudLight.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.White;
            this.label5.Location = new System.Drawing.Point(10, 229);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(79, 20);
            this.label5.TabIndex = 30;
            this.label5.Text = "亮         度:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(10, 13);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(73, 20);
            this.label4.TabIndex = 29;
            this.label4.Text = "旋转角度:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.ForeColor = System.Drawing.Color.White;
            this.label7.Location = new System.Drawing.Point(10, 186);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(79, 20);
            this.label7.TabIndex = 27;
            this.label7.Text = "尺         寸:";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.pictureBox4);
            this.panel2.Controls.Add(this.pictureBox3);
            this.panel2.Controls.Add(this.pictureBox2);
            this.panel2.Controls.Add(this.pictureBox1);
            this.panel2.Controls.Add(this.rb3);
            this.panel2.Controls.Add(this.rb1);
            this.panel2.Controls.Add(this.rb2);
            this.panel2.Controls.Add(this.rb0);
            this.panel2.Controls.Add(this.label6);
            this.panel2.Controls.Add(this.label7);
            this.panel2.Controls.Add(this.nudHight);
            this.panel2.Controls.Add(this.label4);
            this.panel2.Controls.Add(this.label5);
            this.panel2.Controls.Add(this.nudLight);
            this.panel2.Controls.Add(this.nudWidth);
            this.panel2.Location = new System.Drawing.Point(233, 136);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(333, 282);
            this.panel2.TabIndex = 35;
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = global::SharkSmart.Properties.Resources._270;
            this.pictureBox4.Location = new System.Drawing.Point(201, 89);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(35, 35);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox4.TabIndex = 42;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::SharkSmart.Properties.Resources._180;
            this.pictureBox3.Location = new System.Drawing.Point(115, 89);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(35, 35);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox3.TabIndex = 41;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::SharkSmart.Properties.Resources._90;
            this.pictureBox2.Location = new System.Drawing.Point(201, 13);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(35, 35);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox2.TabIndex = 40;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::SharkSmart.Properties.Resources._0;
            this.pictureBox1.Location = new System.Drawing.Point(114, 13);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(35, 35);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 39;
            this.pictureBox1.TabStop = false;
            // 
            // rb3
            // 
            this.rb3.AutoSize = true;
            this.rb3.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.rb3.Location = new System.Drawing.Point(201, 135);
            this.rb3.Name = "rb3";
            this.rb3.Size = new System.Drawing.Size(57, 24);
            this.rb3.TabIndex = 38;
            this.rb3.Text = "270°";
            this.rb3.UseVisualStyleBackColor = true;
            this.rb3.CheckedChanged += new System.EventHandler(this.Rb1_CheckedChanged);
            // 
            // rb1
            // 
            this.rb1.AutoSize = true;
            this.rb1.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.rb1.Location = new System.Drawing.Point(201, 59);
            this.rb1.Name = "rb1";
            this.rb1.Size = new System.Drawing.Size(49, 24);
            this.rb1.TabIndex = 37;
            this.rb1.Text = "90°";
            this.rb1.UseVisualStyleBackColor = true;
            this.rb1.CheckedChanged += new System.EventHandler(this.Rb1_CheckedChanged);
            // 
            // rb2
            // 
            this.rb2.AutoSize = true;
            this.rb2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.rb2.Cursor = System.Windows.Forms.Cursors.Default;
            this.rb2.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.rb2.Location = new System.Drawing.Point(114, 135);
            this.rb2.Name = "rb2";
            this.rb2.Size = new System.Drawing.Size(57, 24);
            this.rb2.TabIndex = 36;
            this.rb2.Text = "180°";
            this.rb2.UseVisualStyleBackColor = true;
            // 
            // rb0
            // 
            this.rb0.AutoSize = true;
            this.rb0.Checked = true;
            this.rb0.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.rb0.Location = new System.Drawing.Point(114, 59);
            this.rb0.Name = "rb0";
            this.rb0.Size = new System.Drawing.Size(41, 24);
            this.rb0.TabIndex = 35;
            this.rb0.TabStop = true;
            this.rb0.Text = "0°";
            this.rb0.UseVisualStyleBackColor = true;
            // 
            // tbName
            // 
            this.tbName.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
            this.tbName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbName.Dock = System.Windows.Forms.DockStyle.Top;
            this.tbName.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
            this.tbName.ForeColor = System.Drawing.Color.White;
            this.tbName.Location = new System.Drawing.Point(5, 5);
            this.tbName.MaxLength = 100;
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(232, 17);
            this.tbName.TabIndex = 0;
            // 
            // panel3
            // 
            this.panel3.AutoSize = true;
            this.panel3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
            this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel3.Controls.Add(this.tbName);
            this.panel3.Location = new System.Drawing.Point(322, 93);
            this.panel3.Name = "panel3";
            this.panel3.Padding = new System.Windows.Forms.Padding(5);
            this.panel3.Size = new System.Drawing.Size(244, 29);
            this.panel3.TabIndex = 36;
            // 
            // FrmAddUnit
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
            this.CaptionHeight = 60;
            this.ClientSize = new System.Drawing.Size(600, 500);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.btnCreate);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.panel2);
            this.Font = new System.Drawing.Font("微软雅黑", 25F);
            this.ForeColor = System.Drawing.Color.White;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(600, 500);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(400, 300);
            this.Name = "FrmAddUnit";
            this.Padding = new System.Windows.Forms.Padding(4);
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Load += new System.EventHandler(this.FrmAddUnit_Load);
            this.Controls.SetChildIndex(this.panel2, 0);
            this.Controls.SetChildIndex(this.btnCancel, 0);
            this.Controls.SetChildIndex(this.TitleBar, 0);
            this.Controls.SetChildIndex(this.panel1, 0);
            this.Controls.SetChildIndex(this.label2, 0);
            this.Controls.SetChildIndex(this.btnCreate, 0);
            this.Controls.SetChildIndex(this.panel3, 0);
            this.TitleBar.ResumeLayout(false);
            this.TitleBar.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudHight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudLight)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown nudHight;
        private System.Windows.Forms.NumericUpDown nudWidth;
        private System.Windows.Forms.NumericUpDown nudLight;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label7;
        private EVTechnology.Controls.EVIconView ivProject;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RadioButton rb0;
        private System.Windows.Forms.RadioButton rb3;
        private System.Windows.Forms.RadioButton rb1;
        private System.Windows.Forms.RadioButton rb2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.PictureBox pictureBox3;
		private System.Windows.Forms.TextBox tbName;
		private System.Windows.Forms.Panel panel3;
	}
}