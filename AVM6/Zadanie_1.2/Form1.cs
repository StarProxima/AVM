using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms;
using ZedGraph;

namespace AVM6
{

    

    public partial class Form1 : Form
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FreeConsole();

        static double f(double x, double y)
        {
            return 4 * x - 2 * y;
            //return Math.Pow(Math.E, x) - y;
            //return 2*x*x-7*y;
            //return x * Math.Pow(Math.E, -x) - 5 * x * y;
        }

        static double f0(double x)
        {
            return Math.Pow(Math.E, -2*x) + 2*x -1;
            //return -0.5d*Math.Pow(Math.E, -x) + Math.Pow(Math.E, x)/2;
            
        }

        double step = 0.1d;
        int n = 10;
        double startX = 0, startY = 0;
        
        private void Eiler()
        {
            GraphPane my_Pane = zedGraphControl.GraphPane;

            PointPairList list = new PointPairList();



            double x = startX, y = startY;
            for (int i = 0; i < n + 1; i++)
            {
                list.Add(x, y);
                y = y + step * f(x, y);
                x += step;
            }

            my_Pane.AddCurve("Eiler", list, Color.Red, SymbolType.Circle);
            zedGraphControl.AxisChange();
            zedGraphControl.Invalidate();
            
        }



        private void RungeKutto2()
        {
            GraphPane my_Pane = zedGraphControl.GraphPane;

            PointPairList list = new PointPairList();



            double x = startX, y = startY;
            for (int i = 0; i < n + 1; i++)
            {
                list.Add(x, y);
                double d = f(x, y);
                y = y + (step / 2) * (d + f(x + step, y + step * d));
                x += step;
            }

            my_Pane.AddCurve("RungeKutto2", list, Color.Yellow, SymbolType.Circle);
            zedGraphControl.AxisChange();
            zedGraphControl.Invalidate();
           
        }
        private void Original()
        {
            GraphPane my_Pane = zedGraphControl.GraphPane;

            PointPairList list = new PointPairList();



            
            for (double x = 0; x < 1; x+=0.01d)
            {
                list.Add(x, f0(x));
            }

            my_Pane.AddCurve("Original", list, Color.Blue, SymbolType.None);
            zedGraphControl.AxisChange();
            zedGraphControl.Invalidate();

        }

        public Form1()
        {
            InitializeComponent();
            Draw();
        }

        
        

        

        public void Draw()
        {
            
            if (AllocConsole())
            {
                FreeConsole();
            }
            

            GraphPane myPane = zedGraphControl.GraphPane;
            myPane.Title.Text = "Cubic Spline Interpolation";
            myPane.XAxis.Title.Text = "Axis Х";
            myPane.YAxis.Title.Text = "Axis Y";
            myPane.XAxis.MajorGrid.IsVisible = true;
            myPane.YAxis.MajorGrid.IsVisible = true;
            myPane.YAxis.MinorGrid.IsVisible = true;
            myPane.XAxis.MinorGrid.IsVisible = true;
            myPane.XAxis.Scale.Min = -1;
            myPane.XAxis.Scale.Max = 1;

            Eiler();
            RungeKutto2();
            Original();









        }
    }
}