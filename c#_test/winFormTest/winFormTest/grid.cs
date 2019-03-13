using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace winFormTest
{
    public enum select
    {
        SAT_1,
        SAT_2,
        SAT_3
    };
    public partial class grid : Form
    {
        List<select> rSelection;

        public grid()
        {
            rSelection = new List<select>();
            InitializeComponent();
        }

        private void grid_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //             if (openFileDialog1.ShowDialog() == DialogResult.OK)
            //             {
            //                 string szPath = openFileDialog1.FileName;
            //                 this.textBox1.Text = szPath;
            //                 Dictionary<string, exchangeType> rResult = new Dictionary<string, exchangeType>();
            //                 xmlOperator.readXmlToGrid(szPath, ref rResult);
            //                 this.dataGridView1.DataSource = new BindingSource(rResult, null);
            //             }
            //             List<string> rLIst = new List<string>();
            //             rLIst.Add("1");
            //             rLIst.Add("1");
            //             rLIst.Add("1");
            //             rLIst.Add("1");
            //             rLIst.Add("1");
            //             rLIst.Add("1");
            //             this.dataGridView1.Rows.Clear();
            //             rSelection.Clear();
            //             for (int index = 0; index < rLIst.Count;++index)
            //             {
            //                 int rowsNUm = this.dataGridView1.Rows.Add();
            //                 this.dataGridView1.Rows[rowsNUm].Cells[0].Value = rowsNUm;
            //                 rSelection.Add(select.SAT_1);
            //             }

            string szOldDateStr = "2019/03/01 00:00:00";
            DateTime rDate = new DateTime();
            DateTime.TryParse(szOldDateStr, out rDate);
            string szNewDataString = rDate.ToString("yyyy-MM-dd HH:mm:ss");
            this.textBox1.Text = szNewDataString;
        }

        private void dataGridView1_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (this.dataGridView1.Rows.Count == 0)
                return;
            if (e.ColumnIndex >= 0 && e.RowIndex >= 0)
            {
                if (this.dataGridView1.Columns[e.ColumnIndex].HeaderText == "操作")
                {
                    StringFormat sf = StringFormat.GenericDefault.Clone() as StringFormat;//设置重绘入单元格的字体样式
                    sf.FormatFlags = StringFormatFlags.DisplayFormatControl;
                    sf.Alignment = StringAlignment.Center;
                    sf.LineAlignment = StringAlignment.Center;
                    sf.Trimming = StringTrimming.EllipsisCharacter;

                    e.PaintBackground(e.CellBounds, false);//重绘边框

                    //设置要写入字体的大小
                    System.Drawing.Font myFont = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
                    SizeF sizeDel = e.Graphics.MeasureString("删除", myFont);
                    SizeF sizeMod = e.Graphics.MeasureString("修改", myFont);
                    SizeF sizeLook = e.Graphics.MeasureString("查看", myFont);

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
                    if (rSelection[e.RowIndex] == select.SAT_1)
                    {
                        rRect = rectDel;
                        e.Graphics.DrawString("删除", myFont, Brushes.Blue, rectDel, sf); //绘制“按钮”
                    }
                    else
                    {
                        e.Graphics.DrawString("删除", myFont, Brushes.Black, rectDel, sf); //绘制“按钮”
                    }
                    if (rSelection[e.RowIndex] == select.SAT_2)
                    {
                        rRect = rectMod;

                        e.Graphics.DrawString("修改", myFont, Brushes.Blue, rectMod, sf); //绘制“按钮”
                    }
                    else
                    {
                        e.Graphics.DrawString("修改", myFont, Brushes.Black, rectMod, sf); //绘制“按钮”
                    }
                    if (rSelection[e.RowIndex] == select.SAT_3)
                    {
                        rRect = rectLook;
                        e.Graphics.DrawString("查看", myFont, Brushes.Blue, rectLook, sf); //绘制“按钮”
                    }
                    else
                    {
                        e.Graphics.DrawString("查看", myFont, Brushes.Black, rectLook, sf); //绘制“按钮”
                    }
                    if(rRect != RectangleF.Empty)
                    {
                        rRect.Y += 1;
                        rRect.X += 1;
                        rRect.Height -= 4;
                        rRect.Width -= 4;


                        e.Graphics.DrawRectangle(rPen, Rectangle.Ceiling(rRect ));
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
                if (this.dataGridView1.Columns[e.ColumnIndex].HeaderText == "操作")
                {
                    Graphics g = this.dataGridView1.CreateGraphics();
                    System.Drawing.Font myFont = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
                    SizeF sizeDel = g.MeasureString("删除", myFont);
                    SizeF sizeMod = g.MeasureString("修改", myFont);
                    SizeF sizeLook = g.MeasureString("查看", myFont);
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
                        //e.Graphics.DrawString("删除", myFont, Brushes.Blue, rectDel, sf); //绘制“按钮”
                        //e.Graphics.DrawString("修改", myFont, Brushes.Black, rectMod, sf);
                        //e.Graphics.DrawString("查看", myFont, Brushes.Black, rectLook, sf);
                        rSelection[e.RowIndex] = select.SAT_1;
                    }
                    else if (rectMod.Contains(curPosition))//修改
                    {
                        // e.Graphics.DrawString("删除", myFont, Brushes.Black, rectDel, sf); //绘制“按钮”
                        // e.Graphics.DrawString("修改", myFont, Brushes.Blue, rectMod, sf);
                        //e.Graphics.DrawString("查看", myFont, Brushes.Black, rectLook, sf);
                        rSelection[e.RowIndex] = select.SAT_2;

                    }
                    else if (rectLook.Contains(curPosition))//查看
                    {
                        //e.Graphics.DrawString("删除", myFont, Brushes.Black, rectDel, sf); //绘制“按钮”
                        //e.Graphics.DrawString("修改", myFont, Brushes.Black, rectMod, sf);
                        //e.Graphics.DrawString("查看", myFont, Brushes.Blue, rectLook, sf);
                        rSelection[e.RowIndex] = select.SAT_3;

                    }
                    //点击以后主动刷新，然后绘制不同颜色
                }
            }
        }

        private void dataGridView1_RowsAdded(object sender, DataGridViewRowsAddedEventArgs e)
        {

        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
           if(e.KeyCode == Keys.Enter)
            {
                DataGridViewRowCollection rRows = this.dataGridView1.Rows;
                for (int index = 0; index < rRows.Count; index ++)
                {
                    rRows[index].Selected = rRows[index].Cells[0].Value.ToString() == this.textBox1.Text;
                }
            }
        }
    }
}
