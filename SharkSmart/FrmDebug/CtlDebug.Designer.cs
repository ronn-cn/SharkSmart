namespace SharkSmart
{
    partial class CtlDebug
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CtlDebug));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.tsmTool = new EVTechnology.Common.Controls.ToolStripNoFrame();
            this.tslSpace = new System.Windows.Forms.ToolStripLabel();
            this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.panel4 = new System.Windows.Forms.Panel();
            this.cbCheck = new System.Windows.Forms.CheckBox();
            this.tsmTool.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "process.png");
            this.imageList1.Images.SetKeyName(1, "electrical_sensor.png");
            this.imageList1.Images.SetKeyName(2, "icon_公司信息维护.png");
            this.imageList1.Images.SetKeyName(3, "self_service_kiosk.png");
            this.imageList1.Images.SetKeyName(4, "candle_sticks.png");
            this.imageList1.Images.SetKeyName(5, "services.png");
            this.imageList1.Images.SetKeyName(6, "jurassic.png");
            // 
            // splitter1
            // 
            this.splitter1.Location = new System.Drawing.Point(0, 0);
            this.splitter1.Margin = new System.Windows.Forms.Padding(4);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(4, 508);
            this.splitter1.TabIndex = 9;
            this.splitter1.TabStop = false;
            // 
            // tsmTool
            // 
            this.tsmTool.AutoSize = false;
            this.tsmTool.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
            this.tsmTool.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tsmTool.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F);
            this.tsmTool.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsmTool.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tslSpace,
            this.toolStripLabel4,
            this.toolStripButton1,
            this.toolStripButton2});
            this.tsmTool.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
            this.tsmTool.Location = new System.Drawing.Point(40, 0);
            this.tsmTool.Name = "tsmTool";
            this.tsmTool.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
            this.tsmTool.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.tsmTool.Size = new System.Drawing.Size(803, 75);
            this.tsmTool.TabIndex = 26;
            this.tsmTool.Text = "toolStripNoFrame3";
            // 
            // tslSpace
            // 
            this.tslSpace.AutoSize = false;
            this.tslSpace.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
            this.tslSpace.Name = "tslSpace";
            this.tslSpace.Size = new System.Drawing.Size(30, 22);
            this.tslSpace.Text = "                      ";
            // 
            // toolStripLabel4
            // 
            this.toolStripLabel4.Name = "toolStripLabel4";
            this.toolStripLabel4.Size = new System.Drawing.Size(0, 72);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.AutoSize = false;
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = global::SharkSmart.Properties.Resources.play__2_;
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(50, 50);
            this.toolStripButton1.Text = "一键烧录";
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.AutoSize = false;
            this.toolStripButton2.BackgroundImage = global::SharkSmart.Properties.Resources.cached;
            this.toolStripButton2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(50, 50);
            this.toolStripButton2.Text = "刷新设备列表";
            // 
            // panel1
            // 
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(4, 75);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(843, 433);
            this.panel1.TabIndex = 28;
            // 
            // panel2
            // 
            this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel2.Location = new System.Drawing.Point(0, 508);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(847, 190);
            this.panel2.TabIndex = 29;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.tsmTool);
            this.panel3.Controls.Add(this.panel4);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel3.Location = new System.Drawing.Point(4, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(843, 75);
            this.panel3.TabIndex = 30;
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.cbCheck);
            this.panel4.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel4.Location = new System.Drawing.Point(0, 0);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(40, 75);
            this.panel4.TabIndex = 27;
            // 
            // cbCheck
            // 
            this.cbCheck.AutoSize = true;
            this.cbCheck.Location = new System.Drawing.Point(13, 32);
            this.cbCheck.Name = "cbCheck";
            this.cbCheck.Size = new System.Drawing.Size(15, 14);
            this.cbCheck.TabIndex = 0;
            this.cbCheck.UseVisualStyleBackColor = true;
            this.cbCheck.CheckedChanged += new System.EventHandler(this.CbCheck_CheckedChanged);
            // 
            // CtlDebug
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.splitter1);
            this.Controls.Add(this.panel2);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "CtlDebug";
            this.Size = new System.Drawing.Size(847, 698);
            this.Load += new System.EventHandler(this.CtlDebug_Load);
            this.tsmTool.ResumeLayout(false);
            this.tsmTool.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
		private System.Windows.Forms.Splitter splitter1;
		private System.Windows.Forms.ImageList imageList1;
        private EVTechnology.Common.Controls.ToolStripNoFrame tsmTool;
        private System.Windows.Forms.ToolStripLabel tslSpace;
        private System.Windows.Forms.ToolStripLabel toolStripLabel4;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.CheckBox cbCheck;
    }
}
