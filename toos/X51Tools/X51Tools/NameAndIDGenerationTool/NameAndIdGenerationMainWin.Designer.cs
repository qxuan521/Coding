namespace X51Tools
{
    partial class NameAndIdGenerationMainWin
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.m_rWorkTableSelect_btn = new System.Windows.Forms.Button();
            this.m_rSrcFolderSelect_btn = new System.Windows.Forms.Button();
            this.m_rWorkTableTextBlock = new System.Windows.Forms.TextBox();
            this.m_rSrcTextBlock = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.panel3 = new System.Windows.Forms.Panel();
            this.m_rCheck_btn = new System.Windows.Forms.Button();
            this.m_rFillIn_btn = new System.Windows.Forms.Button();
            this.m_rWorkTableSelect_dialog = new System.Windows.Forms.OpenFileDialog();
            this.m_rSrcTableSelect_dialog = new System.Windows.Forms.FolderBrowserDialog();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.panel3, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(5);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(834, 612);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.m_rWorkTableSelect_btn);
            this.panel1.Controls.Add(this.m_rSrcFolderSelect_btn);
            this.panel1.Controls.Add(this.m_rWorkTableTextBlock);
            this.panel1.Controls.Add(this.m_rSrcTextBlock);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(828, 94);
            this.panel1.TabIndex = 0;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // m_rWorkTableSelect_btn
            // 
            this.m_rWorkTableSelect_btn.Location = new System.Drawing.Point(716, 56);
            this.m_rWorkTableSelect_btn.Name = "m_rWorkTableSelect_btn";
            this.m_rWorkTableSelect_btn.Size = new System.Drawing.Size(97, 23);
            this.m_rWorkTableSelect_btn.TabIndex = 7;
            this.m_rWorkTableSelect_btn.Text = "浏览";
            this.m_rWorkTableSelect_btn.UseVisualStyleBackColor = true;
            this.m_rWorkTableSelect_btn.Click += new System.EventHandler(this.m_rWorkTableSelect_btn_Click);
            // 
            // m_rSrcFolderSelect_btn
            // 
            this.m_rSrcFolderSelect_btn.Location = new System.Drawing.Point(716, 14);
            this.m_rSrcFolderSelect_btn.Name = "m_rSrcFolderSelect_btn";
            this.m_rSrcFolderSelect_btn.Size = new System.Drawing.Size(97, 23);
            this.m_rSrcFolderSelect_btn.TabIndex = 6;
            this.m_rSrcFolderSelect_btn.Text = "浏览";
            this.m_rSrcFolderSelect_btn.UseVisualStyleBackColor = true;
            this.m_rSrcFolderSelect_btn.Click += new System.EventHandler(this.m_rSrcFolderSelect_btn_Click);
            // 
            // m_rWorkTableTextBlock
            // 
            this.m_rWorkTableTextBlock.AllowDrop = true;
            this.m_rWorkTableTextBlock.Location = new System.Drawing.Point(115, 58);
            this.m_rWorkTableTextBlock.Name = "m_rWorkTableTextBlock";
            this.m_rWorkTableTextBlock.Size = new System.Drawing.Size(582, 21);
            this.m_rWorkTableTextBlock.TabIndex = 4;
            this.m_rWorkTableTextBlock.DragDrop += new System.Windows.Forms.DragEventHandler(this.m_rWorkTableTextBlock_DragDrop);
            this.m_rWorkTableTextBlock.DragEnter += new System.Windows.Forms.DragEventHandler(this.m_rWorkTableTextBlock_DragEnter);
            // 
            // m_rSrcTextBlock
            // 
            this.m_rSrcTextBlock.Location = new System.Drawing.Point(115, 16);
            this.m_rSrcTextBlock.Name = "m_rSrcTextBlock";
            this.m_rSrcTextBlock.Size = new System.Drawing.Size(582, 21);
            this.m_rSrcTextBlock.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(25, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "工作表路径：";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "标准文件路径：";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.richTextBox1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 103);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(15, 0, 15, 0);
            this.panel2.Size = new System.Drawing.Size(828, 466);
            this.panel2.TabIndex = 1;
            // 
            // richTextBox1
            // 
            this.richTextBox1.BackColor = System.Drawing.Color.White;
            this.richTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox1.Location = new System.Drawing.Point(15, 0);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.Size = new System.Drawing.Size(798, 466);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.m_rCheck_btn);
            this.panel3.Controls.Add(this.m_rFillIn_btn);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(3, 575);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(828, 34);
            this.panel3.TabIndex = 2;
            // 
            // m_rCheck_btn
            // 
            this.m_rCheck_btn.Location = new System.Drawing.Point(735, 3);
            this.m_rCheck_btn.Name = "m_rCheck_btn";
            this.m_rCheck_btn.Size = new System.Drawing.Size(75, 23);
            this.m_rCheck_btn.TabIndex = 1;
            this.m_rCheck_btn.Text = "检查";
            this.m_rCheck_btn.UseVisualStyleBackColor = true;
            this.m_rCheck_btn.Click += new System.EventHandler(this.m_rCheck_btn_Click);
            // 
            // m_rFillIn_btn
            // 
            this.m_rFillIn_btn.Location = new System.Drawing.Point(630, 3);
            this.m_rFillIn_btn.Name = "m_rFillIn_btn";
            this.m_rFillIn_btn.Size = new System.Drawing.Size(75, 23);
            this.m_rFillIn_btn.TabIndex = 0;
            this.m_rFillIn_btn.Text = "填写";
            this.m_rFillIn_btn.UseVisualStyleBackColor = true;
            this.m_rFillIn_btn.Click += new System.EventHandler(this.m_rFillIn_btn_Click);
            // 
            // m_rWorkTableSelect_dialog
            // 
            this.m_rWorkTableSelect_dialog.FileName = "工作文件选择";
            // 
            // NameAndIdGenerationMainWin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(834, 612);
            this.Controls.Add(this.tableLayoutPanel1);
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(850, 650);
            this.MinimumSize = new System.Drawing.Size(850, 650);
            this.Name = "NameAndIdGenerationMainWin";
            this.Text = "国际化命名与ID配置检查工具";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.NameAndIdGenerationMainWin_FormClosed);
            this.Load += new System.EventHandler(this.NameAndIdGenerationMainWin_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.ResumeLayout(false);

        }


        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button m_rWorkTableSelect_btn;
        private System.Windows.Forms.Button m_rSrcFolderSelect_btn;
        private System.Windows.Forms.TextBox m_rWorkTableTextBlock;
        private System.Windows.Forms.TextBox m_rSrcTextBlock;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Button m_rCheck_btn;
        private System.Windows.Forms.Button m_rFillIn_btn;
        private System.Windows.Forms.OpenFileDialog m_rWorkTableSelect_dialog;
        private System.Windows.Forms.FolderBrowserDialog m_rSrcTableSelect_dialog;
    #endregion
    }
}