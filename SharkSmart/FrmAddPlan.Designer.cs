namespace SharkSmart
{
    partial class FrmAddPlan
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
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.btnCreate = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.tbName = new System.Windows.Forms.TextBox();
			this.panel1 = new System.Windows.Forms.Panel();
			this.cbbEnable = new System.Windows.Forms.ComboBox();
			this.cbbType = new System.Windows.Forms.ComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.panel2 = new System.Windows.Forms.Panel();
			this.TitleBar.SuspendLayout();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// TitleBar
			// 
			this.TitleBar.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
			this.TitleBar.Controls.Add(this.label3);
			this.TitleBar.Location = new System.Drawing.Point(4, 4);
			this.TitleBar.Size = new System.Drawing.Size(522, 60);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label3.ForeColor = System.Drawing.Color.White;
			this.label3.Location = new System.Drawing.Point(63, 21);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(69, 20);
			this.label3.TabIndex = 11;
			this.label3.Text = "新建计划";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label2.ForeColor = System.Drawing.Color.White;
			this.label2.Location = new System.Drawing.Point(35, 57);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(84, 20);
			this.label2.TabIndex = 10;
			this.label2.Text = "计划名称：";
			// 
			// btnCreate
			// 
			this.btnCreate.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(118)))), ((int)(((byte)(164)))), ((int)(((byte)(92)))));
			this.btnCreate.FlatAppearance.BorderSize = 0;
			this.btnCreate.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnCreate.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btnCreate.ForeColor = System.Drawing.Color.White;
			this.btnCreate.Location = new System.Drawing.Point(39, 249);
			this.btnCreate.Name = "btnCreate";
			this.btnCreate.Size = new System.Drawing.Size(180, 36);
			this.btnCreate.TabIndex = 14;
			this.btnCreate.Text = "创  建";
			this.btnCreate.UseVisualStyleBackColor = false;
			this.btnCreate.Click += new System.EventHandler(this.BtnCreate_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(177)))), ((int)(((byte)(68)))), ((int)(((byte)(65)))));
			this.btnCancel.FlatAppearance.BorderSize = 0;
			this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnCancel.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btnCancel.ForeColor = System.Drawing.Color.White;
			this.btnCancel.Location = new System.Drawing.Point(310, 249);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(180, 36);
			this.btnCancel.TabIndex = 15;
			this.btnCancel.Text = "取  消";
			this.btnCancel.UseVisualStyleBackColor = false;
			this.btnCancel.Click += new System.EventHandler(this.BtnCancel_Click);
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
			this.tbName.Size = new System.Drawing.Size(359, 17);
			this.tbName.TabIndex = 0;
			// 
			// panel1
			// 
			this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.panel1.Controls.Add(this.cbbEnable);
			this.panel1.Controls.Add(this.cbbType);
			this.panel1.Controls.Add(this.label4);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.panel2);
			this.panel1.Controls.Add(this.btnCancel);
			this.panel1.Controls.Add(this.btnCreate);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel1.Location = new System.Drawing.Point(4, 64);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(522, 342);
			this.panel1.TabIndex = 1;
			// 
			// cbbEnable
			// 
			this.cbbEnable.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
			this.cbbEnable.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.cbbEnable.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.cbbEnable.ForeColor = System.Drawing.Color.White;
			this.cbbEnable.FormattingEnabled = true;
			this.cbbEnable.Items.AddRange(new object[] {
            "自  动",
            "手  动"});
			this.cbbEnable.Location = new System.Drawing.Point(125, 178);
			this.cbbEnable.Name = "cbbEnable";
			this.cbbEnable.Size = new System.Drawing.Size(371, 28);
			this.cbbEnable.TabIndex = 20;
			this.cbbEnable.Text = "自  动";
			// 
			// cbbType
			// 
			this.cbbType.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
			this.cbbType.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.cbbType.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.cbbType.ForeColor = System.Drawing.Color.White;
			this.cbbType.FormattingEnabled = true;
			this.cbbType.Items.AddRange(new object[] {
            "持续执行",
            "单次执行"});
			this.cbbType.Location = new System.Drawing.Point(125, 115);
			this.cbbType.Name = "cbbType";
			this.cbbType.Size = new System.Drawing.Size(371, 28);
			this.cbbType.TabIndex = 19;
			this.cbbType.Text = "持续执行";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label4.ForeColor = System.Drawing.Color.White;
			this.label4.Location = new System.Drawing.Point(35, 183);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(84, 20);
			this.label4.TabIndex = 18;
			this.label4.Text = "启用方式：";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.ForeColor = System.Drawing.Color.White;
			this.label1.Location = new System.Drawing.Point(35, 120);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(84, 20);
			this.label1.TabIndex = 17;
			this.label1.Text = "计划类型：";
			// 
			// panel2
			// 
			this.panel2.AutoSize = true;
			this.panel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
			this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel2.Controls.Add(this.tbName);
			this.panel2.Location = new System.Drawing.Point(125, 53);
			this.panel2.Name = "panel2";
			this.panel2.Padding = new System.Windows.Forms.Padding(5);
			this.panel2.Size = new System.Drawing.Size(371, 29);
			this.panel2.TabIndex = 16;
			// 
			// FrmAddPlan
			// 
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
			this.BoderColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.CaptionHeight = 60;
			this.ClientSize = new System.Drawing.Size(530, 410);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("微软雅黑", 25F);
			this.ForeColor = System.Drawing.Color.White;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.MaximizeBox = false;
			this.MaximumSize = new System.Drawing.Size(530, 410);
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(530, 410);
			this.Name = "FrmAddPlan";
			this.Padding = new System.Windows.Forms.Padding(4);
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Load += new System.EventHandler(this.FrmAddUnit_Load);
			this.Controls.SetChildIndex(this.TitleBar, 0);
			this.Controls.SetChildIndex(this.panel1, 0);
			this.TitleBar.ResumeLayout(false);
			this.TitleBar.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.panel2.ResumeLayout(false);
			this.panel2.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ComboBox cbbEnable;
		private System.Windows.Forms.ComboBox cbbType;
	}
}