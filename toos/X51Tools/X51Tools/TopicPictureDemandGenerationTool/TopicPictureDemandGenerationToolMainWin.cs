using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using X51Tools.TopicPictureDemandGenerationTool;

namespace X51Tools.TopicPictureDemandGenerationTool
{
    public partial class TopicPictureDemandGenerationToolMainWin : Form
    {
        TableFillIn m_rFunc1Operator = new TableFillIn();

        public TopicPictureDemandGenerationToolMainWin()
        {
            InitializeComponent();
        }

        private void m_rDoFillInBtn_Click(object sender, EventArgs e)
        {
            this.m_rFunc1Operator.setInputExcelPath(this.m_rInputTableBlock.Text);
            this.m_rFunc1Operator.setConfigFilePath(this.m_rConfigurationBlock.Text);
            this.m_rFunc1Operator.setNoSalePath(this.m_rNoSaleTableBlock.Text);
            m_rFunc1Operator.execute(ref this.richTextBox1);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (this.m_rFolderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                string szError = "";
                string szPath = m_rFolderBrowserDialog.SelectedPath;
                this.m_rPairFolderBlock.Text = szPath;
                this.m_rFunc1Operator.setInternationalPath(szPath, ref szError);
            }
        }

        private void m_rInputTableBtn_Click(object sender, EventArgs e)
        {
            if (this.m_rOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rOpenFileDialog.FileName;
                this.m_rInputTableBlock.Text = szPath;
            }
        }

        private void m_rConfigBtn_Click(object sender, EventArgs e)
        {
            if (this.m_rOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rOpenFileDialog.FileName;
                this.m_rInputTableBlock.Text = szPath;
            }
        }

        private void m_rInputTableBlock_DragEnter(object sender, DragEventArgs e)
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

        private void m_rInputTableBlock_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.m_rInputTableBlock.Text = path;
        }

        private void m_rConfigurationBlock_DragEnter(object sender, DragEventArgs e)
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

        private void m_rConfigurationBlock_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.m_rConfigurationBlock.Text = path;
        }

        private void m_rNoSaleTableBtn_Click(object sender, EventArgs e)
        {
            if (this.m_rOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rOpenFileDialog.FileName;
                this.m_rNoSaleTableBlock.Text = szPath;
            }
        }

        private void m_rNoSaleTableBlock_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.m_rNoSaleTableBlock.Text = path;
        }

        private void m_rNoSaleTableBlock_DragEnter(object sender, DragEventArgs e)
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
    }
}
