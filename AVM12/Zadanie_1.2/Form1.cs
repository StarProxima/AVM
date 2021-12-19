using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms;
using ZedGraph;

namespace AVM12
{
    
    class CubicSpline
    {
        SplineParams[] splineArr;

        
        private struct SplineParams
        {
            public double a, b, c, d, x;
        }

        public void Initialization(double[] x, double[] y, int n)
        {
            splineArr = new SplineParams[n];

            for (int i = 0; i < n; ++i)
            {
                splineArr[i].x = x[i];
                splineArr[i].a = y[i];
            }
            splineArr[0].c = 0;
            splineArr[n - 1].c = 0;


            double[] alpha = new double[n - 1];
            double[] beta = new double[n - 1];
            
            alpha[0] = 0.0;
            beta[0] = 0.0;
            double hi, hi1, A, C, B, G, F, z;
            for (int i = 1; i < n - 1; i++)
            {
                hi = x[i] - x[i - 1];
                hi1 = x[i + 1] - x[i];
                A = hi;
                C = 2.0 * (hi + hi1);
                B = hi1;
                F = 6.0 * ((y[i + 1] - y[i]) / hi1 - (y[i] - y[i - 1]) / hi);
                z = (A * alpha[i - 1] + C);
                alpha[i] = -B / z;
                beta[i] = (F - A * beta[i - 1]) / z;
            }

            
            for (int i = n - 2; i > 0; i--)
            {
                splineArr[i].c = alpha[i] * splineArr[i + 1].c + beta[i];
            }

            
            for (int i = n - 1; i > 0; i--)
            {
                hi = x[i] - x[i - 1];
                splineArr[i].d = (splineArr[i].c - splineArr[i - 1].c) / hi;
                splineArr[i].b = hi * (2.0 * splineArr[i].c + splineArr[i - 1].c) / 6.0 + (y[i] - y[i - 1]) / hi;
            }
        }

       
        public double GetValue(double x)
        {
            int n = splineArr.Length;
            SplineParams s;

            if (x <= splineArr[0].x) 
            {
                s = splineArr[0];
            }
            else if (x >= splineArr[n - 1].x) 
            {
                s = splineArr[n - 1];
            }
            else 
            {
                //Бинарный поиск нужного элемента
                int i = 0;
                int j = n - 1;
                while (i + 1 < j)
                {
                    int k = i + (j - i) / 2;
                    if (x <= splineArr[k].x)
                    {
                        j = k;
                    }
                    else
                    {
                        i = k;
                    }
                }
                s = splineArr[j];
            }

            double dx = x - s.x;
            
            return s.a + (s.b + (s.c / 2.0 + s.d * dx / 6.0) * dx) * dx;
        }
    }

    public partial class Form1 : Form
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FreeConsole();


        public Form1()
        {
            InitializeComponent();
            Draw();
        }

        public double f(double x)
        {
            return x * x/2.342f + x + 1.27f;
            //return Math.Sin(1+x);
            //return Math.Sqrt(x / 2f + 5);
            
        }

        

        public void Draw()
        {
            
            if (AllocConsole())
            {
                FreeConsole();
            }
            CubicSpline cubicSpline = new CubicSpline();    

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

            int n = 20;
            double min = -10, max = 10;

            double[] x = new double[n + 1];
            double[] y = new double[n + 1];
            double step = (max-min) / n;




            //Равностоящие узлы
            for (int i = 0; i < n + 1; i++)
            {
                x[i] = min + i * step;
                y[i] = f(x[i]);
            }



            //Узлы Чебышёва
            //x[0] = min; y[0] = f(min);
            //for (int i = 1; i < n + 1; i++)
            //{
            //    x[i] = (min + max) / 2d + (max - min) / 2d * Math.Cos((2 * (double)i - 1) / (n) * 3.14d);
            //    y[i] = f(x[i]);
            //}




            cubicSpline.Initialization(x, y, n+1);

            PointPairList pairList1 = new PointPairList();
            PointPairList pairList2 = new PointPairList();

            
            for (double i = min; i < max; i+=0.01)
            {
                pairList1.Add(i, f(i));
                pairList2.Add(i, cubicSpline.GetValue(i));
            }
                
               
            myPane.AddCurve("f(x)", pairList1, Color.Blue, SymbolType.None);
            myPane.AddCurve("C(x)", pairList2, Color.Red, SymbolType.None);

            zedGraphControl.AxisChange();
            zedGraphControl.Invalidate();
        }
    }
}