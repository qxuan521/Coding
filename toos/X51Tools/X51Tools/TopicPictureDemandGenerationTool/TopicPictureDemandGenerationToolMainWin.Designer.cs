namespace X51Tools.TopicPictureDemandGenerationTool
{
    partial class TopicPictureDemandGenerationToolMainWin
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
            this.tabWidget = new System.Windows.Forms.TabControl();
            this.m_rFillInPage = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button1 = new System.Windows.Forms.Button();
            this.m_rPairFolderBlock = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.m_rInputTableBtn = new System.Windows.Forms.Button();
            this.m_rInputTableBlock = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.m_rConfigBtn = new System.Windows.Forms.Button();
            this.m_rConfigurationBlock = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.panel5 = new System.Windows.Forms.Panel();
            this.m_rDoFillInBtn = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.m_rOpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.m_rFolderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.panel6 = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.m_rNoSaleTableBlock = new System.Windows.Forms.TextBox();
            this.m_rNoSaleTableBtn = new System.Windows.Forms.Button();
            this.tabWidget.SuspendLayout();
            this.m_rFillInPage.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.panel5.SuspendLayout();
            this.panel6.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabWidget
            // 
            this.tabWidget.Controls.Add(this.m_rFillInPage);
            this.tabWidget.Controls.Add(this.tabPage2);
            this.tabWidget.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabWidget.Location = new System.Drawing.Point(0, 0);
            this.tabWidget.Name = "tabWidget";
            this.tabWidget.SelectedIndex = 0;
            this.tabWidget.Size = new System.Drawing.Size(841, 649);
            this.tabWidget.TabIndex = 0;
            // 
            // m_rFillInPage
            // 
            this.m_rFillInPage.Controls.Add(this.tableLayoutPanel1);
            this.m_rFillInPage.Location = new System.Drawing.Point(4, 22);
            this.m_rFillInPage.Name = "m_rFillInPage";
            this.m_rFillInPage.Padding = new System.Windows.Forms.Padding(3);
            this.m_rFillInPage.Size = new System.Drawing.Size(833, 623);
            this.m_rFillInPage.TabIndex = 0;
            this.m_rFillInPage.Text = "输入表填写与配置文件生成";
            this.m_rFillInPage.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.panel3, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.panel4, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.panel5, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.panel6, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 6;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(827, 617);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.m_rPairFolderBlock);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 5);
            this.panel1.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(827, 55);
            this.panel1.TabIndex = 0;
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("微软雅黑", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button1.Location = new System.Drawing.Point(712, 18);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 1, 3, 1);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(94, 29);
            this.button1.TabIndex = 2;
            this.button1.TabStop = false;
            this.button1.Text = "浏    览";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // m_rPairFolderBlock
            // 
            this.m_rPairFolderBlock.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.m_rPairFolderBlock.Location = new System.Drawing.Point(16, 18);
            this.m_rPairFolderBlock.Name = "m_rPairFolderBlock";
            this.m_rPairFolderBlock.Size = new System.Drawing.Size(666, 29);
            this.m_rPairFolderBlock.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(3, 0, 3, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(137, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "物品信息对应表文件夹：";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.m_rInputTableBtn);
            this.panel2.Controls.Add(this.m_rInputTableBlock);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 65);
            this.panel2.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(827, 55);
            this.panel2.TabIndex = 1;
            // 
            // m_rInputTableBtn
            // 
            this.m_rInputTableBtn.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.m_rInputTableBtn.Location = new System.Drawing.Point(712, 16);
            this.m_rInputTableBtn.Name = "m_rInputTableBtn";
            this.m_rInputTableBtn.Size = new System.Drawing.Size(94, 29);
            this.m_rInputTableBtn.TabIndex = 2;
            this.m_rInputTableBtn.Text = "浏    览";
            this.m_rInputTableBtn.UseVisualStyleBackColor = true;
            this.m_rInputTableBtn.Click += new System.EventHandler(this.m_rInputTableBtn_Click);
            // 
            // m_rInputTableBlock
            // 
            this.m_rInputTableBlock.AllowDrop = true;
            this.m_rInputTableBlock.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.m_rInputTableBlock.Location = new System.Drawing.Point(16, 16);
            this.m_rInputTableBlock.Name = "m_rInputTableBlock";
            this.m_rInputTableBlock.Size = new System.Drawing.Size(666, 29);
            this.m_rInputTableBlock.TabIndex = 1;
            this.m_rInputTableBlock.DragDrop += new System.Windows.Forms.DragEventHandler(this.m_rInputTableBlock_DragDrop);
            this.m_rInputTableBlock.DragEnter += new System.Windows.Forms.DragEventHandler(this.m_rInputTableBlock_DragEnter);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "初版输入表：";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.m_rConfigBtn);
            this.panel3.Controls.Add(this.m_rConfigurationBlock);
            this.panel3.Controls.Add(this.label3);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(0, 185);
            this.panel3.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(827, 55);
            this.panel3.TabIndex = 2;
            // 
            // m_rConfigBtn
            // 
            this.m_rConfigBtn.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.m_rConfigBtn.Location = new System.Drawing.Point(712, 16);
            this.m_rConfigBtn.Name = "m_rConfigBtn";
            this.m_rConfigBtn.Size = new System.Drawing.Size(94, 29);
            this.m_rConfigBtn.TabIndex = 0;
            this.m_rConfigBtn.Text = "浏   览";
            this.m_rConfigBtn.UseVisualStyleBackColor = true;
            this.m_rConfigBtn.Click += new System.EventHandler(this.m_rConfigBtn_Click);
            // 
            // m_rConfigurationBlock
            // 
            this.m_rConfigurationBlock.AllowDrop = true;
            this.m_rConfigurationBlock.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.m_rConfigurationBlock.Location = new System.Drawing.Point(16, 16);
            this.m_rConfigurationBlock.Name = "m_rConfigurationBlock";
            this.m_rConfigurationBlock.Size = new System.Drawing.Size(666, 29);
            this.m_rConfigurationBlock.TabIndex = 4;
            this.m_rConfigurationBlock.DragDrop += new System.Windows.Forms.DragEventHandler(this.m_rConfigurationBlock_DragDrop);
            this.m_rConfigurationBlock.DragEnter += new System.Windows.Forms.DragEventHandler(this.m_rConfigurationBlock_DragEnter);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(89, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "配置文件路径：";
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.richTextBox1);
            this.panel4.Controls.Add(this.label4);
            this.panel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel4.Location = new System.Drawing.Point(0, 245);
            this.panel4.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(827, 337);
            this.panel4.TabIndex = 3;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(16, 16);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(790, 359);
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.Text = "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(14, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 0;
            this.label4.Text = "日志：";
            // 
            // panel5
            // 
            this.panel5.Controls.Add(this.m_rDoFillInBtn);
            this.panel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel5.Location = new System.Drawing.Point(0, 587);
            this.panel5.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(827, 30);
            this.panel5.TabIndex = 4;
            // 
            // m_rDoFillInBtn
            // 
            this.m_rDoFillInBtn.Font = new System.Drawing.Font("微软雅黑", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.m_rDoFillInBtn.Location = new System.Drawing.Point(712, 0);
            this.m_rDoFillInBtn.Name = "m_rDoFillInBtn";
            this.m_rDoFillInBtn.Size = new System.Drawing.Size(94, 30);
            this.m_rDoFillInBtn.TabIndex = 0;
            this.m_rDoFillInBtn.Text = "生    成";
            this.m_rDoFillInBtn.UseVisualStyleBackColor = true;
            this.m_rDoFillInBtn.Click += new System.EventHandler(this.m_rDoFillInBtn_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(833, 623);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // m_rOpenFileDialog
            // 
            this.m_rOpenFileDialog.FileName = "打开";
            // 
            // panel6
            // 
            this.panel6.Controls.Add(this.m_rNoSaleTableBtn);
            this.panel6.Controls.Add(this.m_rNoSaleTableBlock);
            this.panel6.Controls.Add(this.label5);
            this.panel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel6.Location = new System.Drawing.Point(0, 125);
            this.panel6.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.panel6.Name = "panel6";
            this.panel6.Size = new System.Drawing.Size(827, 55);
            this.panel6.TabIndex = 5;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(16, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(125, 12);
            this.label5.TabIndex = 0;
            this.label5.Text = "无优惠组件列表文件：";
            // 
            // m_rNoSaleTableBlock
            // 
            this.m_rNoSaleTableBlock.AllowDrop = true;
            this.m_rNoSaleTableBlock.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.m_rNoSaleTableBlock.Location = new System.Drawing.Point(18, 15);
            this.m_rNoSaleTableBlock.Name = "m_rNoSaleTableBlock";
            this.m_rNoSaleTableBlock.Size = new System.Drawing.Size(664, 29);
            this.m_rNoSaleTableBlock.TabIndex = 1;
            this.m_rNoSaleTableBlock.DragDrop += new System.Windows.Forms.DragEventHandler(this.m_rNoSaleTableBlock_DragDrop);
            this.m_rNoSaleTableBlock.DragEnter += new System.Windows.Forms.DragEventHandler(this.m_rNoSaleTableBlock_DragEnter);
            // 
            // m_rNoSaleTableBtn
            // 
            this.m_rNoSaleTableBtn.Font = new System.Drawing.Font("微软雅黑", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.m_rNoSaleTableBtn.Location = new System.Drawing.Point(712, 15);
            this.m_rNoSaleTableBtn.Name = "m_rNoSaleTableBtn";
            this.m_rNoSaleTableBtn.Size = new System.Drawing.Size(94, 29);
            this.m_rNoSaleTableBtn.TabIndex = 2;
            this.m_rNoSaleTableBtn.Text = "浏    览";
            this.m_rNoSaleTableBtn.UseVisualStyleBackColor = true;
            this.m_rNoSaleTableBtn.Click += new System.EventHandler(this.m_rNoSaleTableBtn_Click);
            // 
            // TopicPictureDemandGenerationToolMainWin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(841, 649);
            this.Controls.Add(this.tabWidget);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "TopicPictureDemandGenerationToolMainWin";
            this.Text = "TopicPictureDemandGenerationToolMainWin";
            this.tabWidget.ResumeLayout(false);
            this.m_rFillInPage.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.panel5.ResumeLayout(false);
            this.panel6.ResumeLayout(false);
            this.panel6.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabWidget;
        private System.Windows.Forms.TabPage m_rFillInPage;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox m_rPairFolderBlock;
        private System.Windows.Forms.Button m_rInputTableBtn;
        private System.Windows.Forms.TextBox m_rInputTableBlock;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button m_rConfigBtn;
        private System.Windows.Forms.TextBox m_rConfigurationBlock;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.Button m_rDoFillInBtn;
        private System.Windows.Forms.OpenFileDialog m_rOpenFileDialog;
        private System.Windows.Forms.FolderBrowserDialog m_rFolderBrowserDialog;
        private System.Windows.Forms.Panel panel6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button m_rNoSaleTableBtn;
        private System.Windows.Forms.TextBox m_rNoSaleTableBlock;
    }
}