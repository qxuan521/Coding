﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace X51Tools
{
    public partial class MainWin : Form
    {
        public MainWin()
        {
            InitializeComponent();
        }

        private void m_NameAndIDGenerationTool_Btn_Click(object sender, EventArgs e)
        {
            X51Tools.NameAndIdGenerationMainWin rTool = new NameAndIdGenerationMainWin();
            rTool.Show();
        }
    }
}
