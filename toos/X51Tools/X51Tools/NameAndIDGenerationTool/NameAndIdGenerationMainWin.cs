using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using X51Tools.NameAndIDGenerationTool;

namespace X51Tools
{
    public partial class NameAndIdGenerationMainWin : Form
    {
        DataResolve m_rDataResolve;
        public NameAndIdGenerationMainWin()
        {
            InitializeComponent();
            m_rDataResolve = new DataResolve();
        }
        private void m_rSrcFolderSelect_btn_Click(object sender, EventArgs e)
        {
            if (this.m_rSrcTableSelect_dialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rSrcTableSelect_dialog.SelectedPath;
                this.m_rSrcTextBlock.Text = szPath;
                this.m_rDataResolve.initialize(szPath, ref this.richTextBox1);
            }
        }

        private void m_rFillIn_btn_Click(object sender, EventArgs e)
        {
            this.richTextBox1.Clear();
            this.m_rDataResolve.completionData(m_rWorkTableTextBlock.Text,  ref this.richTextBox1);
            MessageBox.Show("填写完成！！");
        }

        private void m_rWorkTableSelect_btn_Click(object sender, EventArgs e)
        {
            if (this.m_rWorkTableSelect_dialog.ShowDialog() == DialogResult.OK)
            {
                this.m_rWorkTableTextBlock.Text = m_rWorkTableSelect_dialog.FileName;
            }
        }
        private void m_rCheck_btn_Click(object sender, EventArgs e)
        {
            this.richTextBox1.Clear();
            this.m_rDataResolve.checkData(m_rWorkTableTextBlock.Text, ref this.richTextBox1);
            MessageBox.Show("检查结束！！");
        }

        private void m_rWorkTableTextBlock_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Link;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void m_rWorkTableTextBlock_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            m_rWorkTableTextBlock.Text = path;
        }

        private void NameAndIdGenerationMainWin_FormClosed(object sender, FormClosedEventArgs e)
        {
            m_rDataResolve.Dispose();
            this.Dispose();
        }

        private void NameAndIdGenerationMainWin_Load(object sender, EventArgs e)
        {
            string szSrcPath = m_rDataResolve.getSrcPath();
            if (szSrcPath != "" && szSrcPath != null)
            {
                this.m_rSrcTextBlock.Text = szSrcPath;
                this.m_rDataResolve.initialize(szSrcPath, ref this.richTextBox1);
            }
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
