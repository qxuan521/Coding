using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NameAndIDGenerationTool
{
    public partial class Form1 : Form
    {
        DataResolve m_rDataResolve;
        public Form1()
        {
            InitializeComponent();
            m_rDataResolve = new DataResolve();
        }
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void m_rSrcFolderSelect_btn_Click(object sender, EventArgs e)
        {
            if (this.m_rSrcTableSelect_dialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rSrcTableSelect_dialog.SelectedPath;
                this.m_rSrcTextBlock.Text = szPath;
                this.m_rDataResolve.initialize(szPath, ref this.richTextBox1);
                this.m_rDataResolve.testFunc(ref this.richTextBox1);
            }
        }
    }
}
