using System;
using System.Collections.Generic;
using System.Text;

public class CirclesSeparation
{
    static Random rnd = new Random();

    const double LIMIT = 9.8;
    const double e = 1.0e-9;
    const double ee = 1.0e-9 + 1.0e-10;

    int N;

    double[] x0;
    double[] y0;
    double[] r0;
    double[] m0;

    double[] x1;
    double[] y1;

    double[] x2;
    double[] y2;

    double[] fxy;

    int[] am;

    int NN;

    public double[] minimumWork(double[] x, double[] y, double[] r, double[] m)
    {
        DateTime started = DateTime.Now;
        N = x.Length;

        x0 = new double[N];
        y0 = new double[N];
        r0 = new double[N];
        m0 = new double[N];

        x1 = new double[N];
        y1 = new double[N];

        x2 = new double[N];
        y2 = new double[N];

        am = new int[N];

        for (int i = 0; i < N; i++)
        {
            x0[i] = x[i];
            y0[i] = y[i];
            r0[i] = r[i];
            m0[i] = m[i];

            am[i] = i;
        }

        fxy = new double[N * 2];

        NN = 0;

        int way = 0;
        double score2 = estimateScore(0);
        for (int i = 0; i < N; i++)
        {
            fxy[i * 2] = x1[i];
            fxy[i * 2 + 1] = y1[i];
            x2[i] = x1[i];
            y2[i] = y1[i];
        }
        if (DateTime.Now.Subtract(started).TotalSeconds > 3) return fxy;

        double score1 = estimateScore(1);
        if (score1 < score2)
        {
            way = 1;
            score2 = score1;
            for (int i = 0; i < N; i++)
            {
                fxy[i * 2] = x1[i];
                fxy[i * 2 + 1] = y1[i];
                x2[i] = x1[i];
                y2[i] = y1[i];
            }
        }
        if (DateTime.Now.Subtract(started).TotalSeconds > 5) return fxy;

        score1 = estimateScore(2);
        if (score1 < score2)
        {
            way = 2;
            score2 = score1;
            for (int i = 0; i < N; i++)
            {
                fxy[i * 2] = x1[i];
                fxy[i * 2 + 1] = y1[i];
                x2[i] = x1[i];
                y2[i] = y1[i];
            }
        }
        if (DateTime.Now.Subtract(started).TotalSeconds > 7) return fxy;

        switch (way)
        {
            case 0:
                Array.Sort(am, mcompare1);
                break;
            case 1:
                Array.Sort(am, mcompare2);
                break;
            case 2:
                Array.Sort(am, mcompare3);
                break;
        }

        bool timeIsOver = false;

        double[] rrD0 = new double[N];

        double d6 = LIMIT - DateTime.Now.Subtract(started).TotalSeconds - (N > 450 ? 1 : 0);
        int j100 = 100;

        NN = 0;

        while (LIMIT > DateTime.Now.Subtract(started).TotalSeconds)
        {
            j100 = 100 + 10 * NN;

            x1[am[0]] = x0[am[0]];
            y1[am[0]] = y0[am[0]];

            for (int index = 1; index < N; index++)
            {
                if (LIMIT < DateTime.Now.Subtract(started).TotalSeconds)
                {
                    timeIsOver = true;
                    break;
                }

                x1[am[index]] = x0[am[index]];
                y1[am[index]] = y0[am[index]];

                for (int i = 0; i < index; i++) rrD0[i] = rrDistance(x1[am[i]], y1[am[i]], r0[am[i]], x1[am[index]], y1[am[index]], r0[am[index]]);

                bool good = true;
                for (int i = 0; i < index; i++)
                    if (rrD0[i] < 0)
                    {
                        good = false;
                        break;
                    }
                if (good) continue;

                double d = r0[am[index]] / d6 + ee;

                for (int i = 0; ; i++)
                {
                    if (LIMIT < DateTime.Now.Subtract(started).TotalSeconds)
                    {
                        timeIsOver = true;
                        break;
                    }

                    for (int k = 0; k < index; k++) rrD0[k] -= d;

                    for (int j = 0; j < j100; j++)
                    {
                        double a = Math.PI * 2 * rnd.NextDouble();
                        x1[am[index]] = x0[am[index]] + d * i * Math.Cos(a);
                        y1[am[index]] = y0[am[index]] + d * i * Math.Sin(a);

                        good = true;
                        for (int k = 0; k < index; k++)
                            if (rrD0[k] < 0 && rrDistance(x1[am[k]], y1[am[k]], r0[am[k]], x1[am[index]], y1[am[index]], r0[am[index]]) < 0)
                            {
                                good = false;
                                break;
                            }
                        if (good)
                        {
                            double ofs = d * i;

                            for (int k = 0; k < index; k++)
                                if (rrD0[k] < ofs)
                                {
                                    double rd = rrDistance(x1[am[k]], y1[am[k]], r0[am[k]], x1[am[index]], y1[am[index]], r0[am[index]]);
                                    if (rd < ofs) ofs = rd;
                                }
                            x1[am[index]] = x0[am[index]] + (d * i - ofs) * Math.Cos(a);
                            y1[am[index]] = y0[am[index]] + (d * i - ofs) * Math.Sin(a);

                            break;
                        }
                    }
                    if (good) break;
                }
            }

            if (!timeIsOver)
            {
                score1 = getScore(x0, y0, m0, x1, y1);
                if (score1 < score2)
                {
                    for (int i = 0; i < N; i++)
                    {
                        x2[i] = x1[i];
                        y2[i] = y1[i];
                    }

                    score2 = score1;
                }

                NN++;
            }
        }

        fxy = new double[N * 2];
        for (int i = 0; i < N; i++)
        {
            fxy[i * 2] = x2[i];
            fxy[i * 2 + 1] = y2[i];
        }
        return fxy;
    }

    double estimateScore(int way)
    {
        double[] rrD0 = new double[N];

        switch (way)
        {
            case 0:
                Array.Sort(am, mcompare1);
                break;
            case 1:
                Array.Sort(am, mcompare2);
                break;
            case 2:
                Array.Sort(am, mcompare3);
                break;
        }

        double d6 = 2;
        int j100 = 100;

        x1[am[0]] = x0[am[0]];
        y1[am[0]] = y0[am[0]];

        for (int index = 1; index < N; index++)
        {
            x1[am[index]] = x0[am[index]];
            y1[am[index]] = y0[am[index]];

            for (int i = 0; i < index; i++) rrD0[i] = rrDistance(x1[am[i]], y1[am[i]], r0[am[i]], x1[am[index]], y1[am[index]], r0[am[index]]);

            bool good = true;
            for (int i = 0; i < index; i++)
            {
                if (rrD0[i] < 0)
                {
                    good = false;
                    break;
                }
            }
            if (good) continue;

            double d = r0[am[index]] / d6 + ee;

            for (int i = 0; ; i++)
            {
                for (int k = 0; k < index; k++) rrD0[k] -= d;

                for (int j = 0; j < j100; j++)
                {
                    double a = Math.PI * 2 * rnd.NextDouble();
                    x1[am[index]] = x0[am[index]] + d * i * Math.Cos(a);
                    y1[am[index]] = y0[am[index]] + d * i * Math.Sin(a);

                    good = true;
                    for (int k = 0; k < index; k++)
                        if (rrD0[k] < 0 && rrDistance(x1[am[k]], y1[am[k]], r0[am[k]], x1[am[index]], y1[am[index]], r0[am[index]]) < 0)
                        {
                            good = false;
                            break;
                        }
                    if (good) break;
                }
                if (good) break;
            }
        }

        return getScore(x0, y0, m0, x1, y1);
    }

    int mcompare1(int c1, int c2)
    {
        if (m0[c1] > m0[c2]) return -1;
        else if (m0[c1] < m0[c2]) return 1;
        else return 0;
    }

    int mcompare2(int c1, int c2)
    {
        double p1 = m0[c1] / Math.Sqrt(r0[c1]);
        double p2 = m0[c2] / Math.Sqrt(r0[c2]);

        if (p1 > p2) return -1;
        else if (p1 < p2) return 1;
        else return 0;
    }

    int mcompare3(int c1, int c2)
    {
        double p1 = m0[c1] / r0[c1];
        double p2 = m0[c2] / r0[c2];

        if (p1 > p2) return -1;
        else if (p1 < p2) return 1;
        else return 0;
    }

    static double rrDistance(double x1, double y1, double r1, double x2, double y2, double r2)
    {
        double xx = x2 - x1;
        double yy = y2 - y1;
        return Math.Sqrt(xx * xx + yy * yy) - (r1 + r2 + e);
    }

    static double getScore(double[] x, double[] y, double[] m, double[] x1, double[] y1)
    {
        double score = 0;
        for (int i = 0; i < x.Length; i++)
        {
            double xx = x1[i] - x[i];
            double yy = y1[i] - y[i];
            score += m[i] * Math.Sqrt(xx * xx + yy * yy);
        }
        return score;
    }

    static double getScore(double[] x, double[] y, double[] m, double[] fxy)
    {
        double score = 0;
        for (int i = 0; i < x.Length; i++)
        {
            double xx = fxy[i * 2] - x[i];
            double yy = fxy[i * 2 + 1] - y[i];
            score += m[i] * Math.Sqrt(xx * xx + yy * yy);
        }
        return score;
    }


}
