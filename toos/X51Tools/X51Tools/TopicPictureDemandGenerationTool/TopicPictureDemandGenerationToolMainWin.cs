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
    public enum ExchangeState
    {
        Close,
        Sale,
        Normal,
        Error
    };
    public partial class TopicPictureDemandGenerationToolMainWin : Form
    {
        TableFillIn m_rFunc1Operator = new TableFillIn();
        SaleChange m_rSaleChange = new SaleChange();

        public TopicPictureDemandGenerationToolMainWin()
        {
            InitializeComponent();
        }

        private void m_rDoFillInBtn_Click(object sender, EventArgs e)
        {
            this.m_rFunc1Operator.setInputExcelPath(this.m_rInputTableBlock.Text);
            this.m_rFunc1Operator.setConfigFilePath(this.m_rConfigurationBlock.Text);
            this.m_rFunc1Operator.setNoSalePath(this.m_rNoSaleTableBlock.Text);
            getSaveAsPath rGetPathFunc = new getSaveAsPath(() => 
            {
                if(this.m_rSaveAsDialog.ShowDialog() == DialogResult.OK)
                {
                    return m_rSaveAsDialog.FileName;
                }
                else
                {
                    return string.Empty;
                }
            });
            m_rFunc1Operator.execute(ref this.richTextBox1, rGetPathFunc);
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
        //--------------------------func2-------------------------------------------------------------------------------
        List<ExchangeState> rSelection = new List<ExchangeState>();
        string szLastSearch = string.Empty;
        private void dataGridView1_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (this.dataGridView1.Rows.Count == 0)
                return;
            if (e.ColumnIndex >= 0 && e.RowIndex >= 0)
            {
                if (this.dataGridView1.Columns[e.ColumnIndex].Name == "Column2")
                {
                    StringFormat sf = StringFormat.GenericDefault.Clone() as StringFormat;//设置重绘入单元格的字体样式
                    sf.FormatFlags = StringFormatFlags.DisplayFormatControl;
                    sf.Alignment = StringAlignment.Center;
                    sf.LineAlignment = StringAlignment.Center;
                    sf.Trimming = StringTrimming.EllipsisCharacter;

                    e.PaintBackground(e.CellBounds, false);//重绘边框

                    //设置要写入字体的大小
                    System.Drawing.Font myFont = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
                    SizeF sizeDel = e.Graphics.MeasureString("关闭兑换", myFont);
                    SizeF sizeMod = e.Graphics.MeasureString("首次兑换优惠", myFont);
                    SizeF sizeLook = e.Graphics.MeasureString("普通兑换", myFont);

                    float fDel = sizeDel.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width); //
                    float fMod = sizeMod.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);
                    float fLook = sizeLook.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);

                    //设置每个“按钮的边界”
                    RectangleF rectDel = new RectangleF(e.CellBounds.Left, e.CellBounds.Top, e.CellBounds.Width * fDel, e.CellBounds.Height);
                    RectangleF rectMod = new RectangleF(rectDel.Right, e.CellBounds.Top, e.CellBounds.Width * fMod, e.CellBounds.Height);
                    RectangleF rectLook = new RectangleF(rectMod.Right, e.CellBounds.Top, e.CellBounds.Width * fLook, e.CellBounds.Height);
                    RectangleF rtemp = new RectangleF(2, 2, 2, 2);
                    Pen rPen = new Pen(Color.Green);
                    RectangleF rRect = RectangleF.Empty;
                    if (rSelection[e.RowIndex] == ExchangeState.Close)
                    {
                        rRect = rectDel;
                        e.Graphics.DrawString("关闭兑换", myFont, Brushes.Blue, rectDel, sf); //绘制“按钮”
                    }
                    else
                    {
                        e.Graphics.DrawString("关闭兑换", myFont, Brushes.Black, rectDel, sf); //绘制“按钮”
                    }
                    if (rSelection[e.RowIndex] == ExchangeState.Sale)
                    {
                        rRect = rectMod;

                        e.Graphics.DrawString("首次兑换优惠", myFont, Brushes.Blue, rectMod, sf); //绘制“按钮”
                    }
                    else
                    {
                        e.Graphics.DrawString("首次兑换优惠", myFont, Brushes.Black, rectMod, sf); //绘制“按钮”
                    }
                    if (rSelection[e.RowIndex] == ExchangeState.Normal)
                    {
                        rRect = rectLook;
                        e.Graphics.DrawString("普通兑换", myFont, Brushes.Blue, rectLook, sf); //绘制“按钮”
                    }
                    else
                    {
                        e.Graphics.DrawString("普通兑换", myFont, Brushes.Black, rectLook, sf); //绘制“按钮”
                    }
                    if (rRect != RectangleF.Empty)
                    {
                        rRect.Y += 1;
                        rRect.X += 1;
                        rRect.Height -= 4;
                        rRect.Width -= 4;


                        e.Graphics.DrawRectangle(rPen, Rectangle.Ceiling(rRect));
                    }

                    e.Handled = true;
                }
            }
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.ColumnIndex >= 0 && e.RowIndex >= 0)
            {
                Point curPosition = e.Location;//当前鼠标在当前单元格中的坐标
                if (this.dataGridView1.Columns[e.ColumnIndex].Name == "Column2")
                {
                    Graphics g = this.dataGridView1.CreateGraphics();
                    System.Drawing.Font myFont = new System.Drawing.Font("华文雅黑", 9F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
                    SizeF sizeDel = g.MeasureString("关闭兑换", myFont);
                    SizeF sizeMod = g.MeasureString("首次兑换优惠", myFont);
                    SizeF sizeLook = g.MeasureString("普通兑换", myFont);
                    float fDel = sizeDel.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);
                    float fMod = sizeMod.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);
                    float fLook = sizeLook.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);

                    Rectangle rectTotal = new Rectangle(0, 0, this.dataGridView1.Columns[e.ColumnIndex].Width, this.dataGridView1.Rows[e.RowIndex].Height);
                    RectangleF rectDel = new RectangleF(rectTotal.Left, rectTotal.Top, rectTotal.Width * fDel, rectTotal.Height);
                    RectangleF rectMod = new RectangleF(rectDel.Right, rectTotal.Top, rectTotal.Width * fMod, rectTotal.Height);
                    RectangleF rectLook = new RectangleF(rectMod.Right, rectTotal.Top, rectTotal.Width * fLook, rectTotal.Height);
                    //判断当前鼠标在哪个“按钮”范围内
                    if (rectDel.Contains(curPosition))//删除
                    {
                        rSelection[e.RowIndex] = ExchangeState.Close;
                    }
                    else if (rectMod.Contains(curPosition))//修改
                    {
                        rSelection[e.RowIndex] = ExchangeState.Sale;

                    }
                    else if (rectLook.Contains(curPosition))//查看
                    {
                        rSelection[e.RowIndex] = ExchangeState.Normal;

                    }
                    //点击以后主动刷新，然后绘制不同颜色
                }
            }
        }

        private void m_rSearchListBtn_Click(object sender, EventArgs e)
        {
            if (this.m_rOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rOpenFileDialog.FileName;
                this.m_rSearchListBlk.Text = szPath;
            }
        }

        private void m_rConfigFunc2Btn_Click(object sender, EventArgs e)
        {
            if (this.m_rOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rOpenFileDialog.FileName;
                this.m_rConfig_func2_block.Text = szPath;
            }
        }

        private void m_rSearchListBlk_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.m_rSearchListBlk.Text = path;
        }

        private void m_rConfig_func2_block_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.m_rConfig_func2_block.Text = path;
        }

        private void m_rSearchListBlk_DragEnter(object sender, DragEventArgs e)
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

        private void m_rConfig_func2_block_DragEnter(object sender, DragEventArgs e)
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

        private void m_rSelectNode_Click(object sender, EventArgs e)
        {
            m_rSaleChange.setConfigPath(this.m_rConfig_func2_block.Text.Trim());
            m_rSaleChange.setSearchlistPath(this.m_rSearchListBlk.Text.Trim());
            m_rSaleChange.search(upDateTable);
        }

        private void upDateTable(Dictionary<string,ExchangeState> rResult)
        {
            this.dataGridView1.Rows.Clear();
            rSelection.Clear();
            foreach (var it in rResult)
            {
                int nRowNum = this.dataGridView1.Rows.Add();
                this.dataGridView1.Rows[nRowNum].Cells["Column1"].Value = it.Key;
                rSelection.Add(it.Value);
            }
        }

        private void m_rChangeNode_Click(object sender, EventArgs e)
        {
            Dictionary<string, ExchangeState> rDataMap = new Dictionary<string, ExchangeState>();
            for(int index = 0; index < this.dataGridView1.RowCount;++index)
            {
                string szID = this.dataGridView1.Rows[index].Cells["Column1"].Value.ToString();
                ExchangeState eState = this.rSelection[index];
                rDataMap.Add(szID, eState);
            }
            getSaveAsPath rGetPathFunc = new getSaveAsPath(() =>
            {
                if (this.m_rSaveAsDialog.ShowDialog() == DialogResult.OK)
                {
                    return m_rSaveAsDialog.FileName;
                }
                else
                {
                    return string.Empty;
                }
            });
            this.m_rSaleChange.alterData(rDataMap, rGetPathFunc);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (this.textBox3.Text.Trim() != string.Empty && this.textBox3.Text.Trim() != szLastSearch)
            {
                DataGridViewRowCollection rRows = this.dataGridView1.Rows;
                for (int index = 0; index < rRows.Count; index++)
                {
                    szLastSearch = this.textBox3.Text.Trim();
                    rRows[index].Selected = rRows[index].Cells[0].Value.ToString() == this.textBox3.Text;
                }
            }
        }

        private void textBox3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (this.textBox3.Text.Trim() != string.Empty &&this.textBox3.Text.Trim() != szLastSearch)
                {
                    DataGridViewRowCollection rRows = this.dataGridView1.Rows;
                    for (int index = 0; index < rRows.Count; index++)
                    {
                        szLastSearch = this.textBox3.Text.Trim();
                        rRows[index].Selected = rRows[index].Cells[0].Value.ToString() == this.textBox3.Text.Trim();
                    }
                }
            }
        }

        private void m_rAllSetOne_Click(object sender, EventArgs e)
        {
            Dictionary<string, ExchangeState> rDataMap = new Dictionary<string, ExchangeState>();
            for (int index = 0; index < this.dataGridView1.RowCount; ++index)
            {
                string szID = this.dataGridView1.Rows[index].Cells["Column1"].Value.ToString();
                rSelection[index] = ExchangeState.Close;
                ExchangeState eState = this.rSelection[index];
                rDataMap.Add(szID, eState);
            }
            upDateTable(rDataMap);
        }

        private void m_rAllSetTwo_Click(object sender, EventArgs e)
        {
            Dictionary<string, ExchangeState> rDataMap = new Dictionary<string, ExchangeState>();
            for (int index = 0; index < this.dataGridView1.RowCount; ++index)
            {
                string szID = this.dataGridView1.Rows[index].Cells["Column1"].Value.ToString();
                rSelection[index] = ExchangeState.Sale;
                ExchangeState eState = this.rSelection[index];
                rDataMap.Add(szID, eState);
            }
            upDateTable(rDataMap);
        }

        private void m_rAllSetThree_Click(object sender, EventArgs e)
        {
            Dictionary<string, ExchangeState> rDataMap = new Dictionary<string, ExchangeState>();
            for (int index = 0; index < this.dataGridView1.RowCount; ++index)
            {
                string szID = this.dataGridView1.Rows[index].Cells["Column1"].Value.ToString();
                rSelection[index] = ExchangeState.Normal;
                ExchangeState eState = this.rSelection[index];
                rDataMap.Add(szID, eState);
            }
            upDateTable(rDataMap);
        }

        private void dataGridView1_Paint(object sender, PaintEventArgs e)
        {

        }
        //-------------------------------------func3-----------------------------------------------------
        GetHistoryData m_rGetHistoryData = new GetHistoryData();
        private void m_rConfigFunc3Block_DragEnter(object sender, DragEventArgs e)
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

        private void m_rConfigFunc3Block_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.m_rConfigFunc3Block.Text = path;
        }

        private void m_rIDListBlock_DragEnter(object sender, DragEventArgs e)
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

        private void m_rIDListBlock_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.m_rIDListBlock.Text = path;
        }

        private void m_rIDListBtn_Click(object sender, EventArgs e)
        {
            if (this.m_rOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rOpenFileDialog.FileName;
                this.m_rIDListBlock.Text = szPath;
            }
        }

        private void m_rConfigFunc3Btn_Click(object sender, EventArgs e)
        {
            if (this.m_rOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rOpenFileDialog.FileName;
                this.m_rIDListBlock.Text = szPath;
            }
        }

        private void m_rOutputHistory_Click(object sender, EventArgs e)
        {
            this.m_rGetHistoryData.setConfigPath(this.m_rConfigFunc3Block.Text.Trim());
            this.m_rGetHistoryData.setSearchList(this.m_rIDListBlock.Text.Trim());
            this.m_rGetHistoryData.excute();
        }
    }
}
