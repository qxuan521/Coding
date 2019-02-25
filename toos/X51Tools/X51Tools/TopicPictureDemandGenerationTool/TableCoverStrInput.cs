using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace X51Tools.TopicPictureDemandGenerationTool
{
    public partial class TableCoverStrInput : Form
    {

        private bool m_bIsCommit = false;

        public TableCoverStrInput()
        {
            InitializeComponent();
        }



        public void initialize(string szName)
        {
            
            this.m_rNameBlock.Text = szName;
        }

        public string getCoverInputStr()
        {
            return this.m_rIDBlock.Text.Trim();
        }

        public bool getCommit()
        {
            return m_bIsCommit;
        }


        private void m_rCommitBtn_Click(object sender, EventArgs e)
        {
            m_bIsCommit = true;
            this.Close();
        }

        private void m_rRejectBtn_Click(object sender, EventArgs e)
        {
            m_bIsCommit = false;
            this.Close();
        }

        private void m_rIDBlock_TextChanged(object sender, EventArgs e)
        {
            this.m_rCommitBtn.Enabled = "" != this.m_rNameBlock.Text.Trim();
        }
    }
}
