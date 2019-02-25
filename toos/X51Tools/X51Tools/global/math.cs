using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace X51Tools.Global
{
    static class X51Math
    {
        static public int X51round(double n1)
        {
            int temp = Convert.ToInt32(n1 * 10) % 10;
            if (temp>4)
            {
                return Convert.ToInt32(System.Math.Ceiling(n1));
            }
            else
            {
                return Convert.ToInt32(System.Math.Floor(n1));
            }
        }
    }
}
