using System;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using ZedGraph;

namespace AVM4
{
    public partial class Form1 : Form
    {
        double eps = 0.001d;
        int n;
        double[] b;
        double[,] A;
        double initialApproximation = 1;
        public Form1()
        {
            InitializeComponent();
            OnLoad();
        }
        static double[,] Input(out int n, out double[] b)
        {
            string str;
            
            using (StreamReader file = new StreamReader("matr.txt"))
            {
                str = file.ReadToEnd();
            }

            n = 0;
            using (StreamReader file = new StreamReader("matr.txt"))
            {
                string tm;
                
                while (true)
                {
                    tm = file.ReadLine();
                    if (tm != null) n++;
                    else break;
                }
                   
                
            }


            b = new double[n];
            double t;
            string[] strings = str.Split('\n');
            string[] values;
            double[,] a = new double[n, n];
            
            for (int i = 0; i < strings.Length; i++)
            {
                values = strings[i].Split(' ');
                for (int j = 0; j < values.Length - 1; j++)
                {
                    t = Convert.ToDouble(values[j]);
                    a[i, j] = t;
                }
                t = Convert.ToDouble(values[n]);
                b[i] = t;
            }
            return a;
        }

        void OnLoad()
        {
            n = 0;
            b = new double[n];
            A = Input(out n, out b);
            


            GraphPane myPane = zedGraphControl1.GraphPane;
            myPane.Title.Text = "Зависимость нормы невязки от номера итерации";
            myPane.XAxis.Title.Text = "Номер итерации";
            myPane.YAxis.Title.Text = "Значение нормы невязки";
            myPane.XAxis.MajorGrid.IsVisible = true;
            myPane.YAxis.MajorGrid.IsVisible = true;
            myPane.YAxis.MinorGrid.IsVisible = true;
            myPane.XAxis.MinorGrid.IsVisible = true;
            myPane.XAxis.Scale.MinorStep = 1;
            myPane.XAxis.Scale.MajorStep = 1;



            Jacobi();
            Relaxatio();
            
        }

        double ResidualNorm(double[] x)
        {
            double[] r = new double[n];
            for (int i = 0; i < n; i++)
            {
                double s = 0;
                for (int j = 0; j < n; j++)
                    s += A[i, j] * x[j];
                r[i] = s - b[i];
            }

            double norm = 0;
            for (int i = 0; i < n; i++)
                norm += r[i] * r[i];
            norm = Math.Sqrt(norm);
            return norm;
        }

        void Jacobi()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            PointPairList points = new PointPairList();
            int k = 0;
            double[] xn = new double[n];
            double norm;
            double[] x = new double[n];
            for (int i = 0; i < n; i++)
                x[i] = initialApproximation;

            do
            {
                points.Add(k, ResidualNorm(x));
                for (int i = 0; i < n; i++)
                {
                    xn[i] = b[i];
                    for (int j = 0; j < n; j++)
                    {
                        if (i != j)
                            xn[i] -= A[i, j] * x[j];
                    }
                    xn[i] /= A[i, i];
                }
                norm = Math.Abs(x[0] - xn[0]);
                for (int i = 0; i < n; i++)
                {
                    if (Math.Abs(x[i] - xn[i]) > norm)
                        norm = Math.Abs(x[i] - xn[i]);
                    x[i] = xn[i];
                }
                k++;
            } while (norm > eps);

            points.Add(k, ResidualNorm(x));
            myPane.AddCurve("JacobiMethod", points, Color.Blue, SymbolType.Circle);

            label8.Text = "";
            for (int i = 0; i < n; i++)
                label8.Text += x[i] + "  ";
            label12.Text = Convert.ToString(k);
        }

        
        void Relaxatio()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            PointPairList points = new PointPairList();
            double[] x = new double[n], xn = new double[n];
            
            
            

            double norma, w = 1.25d;
            int k = 0;
            


            for (int i = 0; i < n; i++)
            {
                xn[i] = initialApproximation;
                x[i] = xn[i];
            }
            do
            {
                points.Add(k, ResidualNorm(x));
                k++;
                norma = 0;
                
                for (int i = 0; i < n; i++)
                {
                    x[i] = b[i];
                    for (int j = 0; j < n; j++)
                    {
                        if (i != j)
                            x[i] = x[i] - A[i, j] * x[j];
                    }
                    x[i] /= A[i, i];

                    x[i] = w * x[i] + (1 - w) * xn[i];

                    if (Math.Abs(x[i] - xn[i]) > norma)
                        norma = Math.Abs(x[i] - xn[i]);
                    xn[i] = x[i];
                }
            }
            while (norma > eps);

            points.Add(k, ResidualNorm(x));
            myPane.AddCurve("RelaxatioMethod", points, Color.Red, SymbolType.Circle);

            label10.Text = "";
            for (int i = 0; i < n; i++)
                label10.Text += x[i] + "  ";
            label6.Text = Convert.ToString(k);


        }
    }
}