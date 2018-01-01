import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.security.SecureRandom;
import java.util.Random;

public class CirclesSeparationVis {
    public static String execCommand = null;
    public static long seed = 1;
    public static boolean vis = true;
    public static int size = 700;

    public static Process solution;

    public static final int MIN_N = 50;
    public static final int MAX_N = 500;

    public static final double MIN_R_FACTOR = 1.0;
    public static final double MAX_R_FACTOR = 5.0;

    public static final double MAX_COORD = 100.0;

    Drawer drawer;
    int N;
    double[] x, y, r, m;
    double[] fx, fy;

    class Drawer extends JFrame {
        public static final int PADDING = 50;
        double minX, maxX, minY, maxY, scale;
        boolean drawTrajectory = true;

        class DrawerKeyListener extends KeyAdapter {
            Drawer parent;
            public DrawerKeyListener(Drawer parent) {
                this.parent = parent;
            }
            public void keyPressed(KeyEvent e) {
                if (e.getKeyChar() == ' ') {
                    drawTrajectory = !drawTrajectory;
                    parent.repaint();
                }
            }
        }

        class DrawerPanel extends JPanel {
            int getX(double x) {
                return (int)Math.round((x - (maxX + minX) / 2) * scale + size / 2.0);
            }

            int getY(double y) {
                return (int)Math.round(((maxY + minY) / 2 - y) * scale + size / 2.0);
            }

            int getL(double len) {
                return (int)Math.round(len * scale);
            }

            public void paint(Graphics g) {
                for (int i=0; i < fx.length; i++) {
                    int col = (int) Math.round(228 - 100 * m[i]);
                    g.setColor(new Color(col, col, 255));
                    g.fillOval(getX(fx[i] - r[i]), getY(fy[i] + r[i]), getL(2 * r[i]) + 1, getL(2 * r[i]) + 1);
                    g.setColor(Color.BLUE);
                    g.drawOval(getX(fx[i] - r[i]), getY(fy[i] + r[i]), getL(2 * r[i]) + 1, getL(2 * r[i]) + 1);
                }

                if (drawTrajectory) {
                    for (int i=0; i < fx.length; i++) {
                        g.setColor(Color.RED);
                        g.drawLine(getX(x[i]), getY(y[i]), getX(fx[i]), getY(fy[i]));
                        g.fillOval(getX(x[i]) - 2, getY(y[i]) - 2, 5, 5);
                        g.fillOval(getX(fx[i]) - 2, getY(fy[i]) - 2, 5, 5);
                    }
                }

                g.setColor(new Color(0, 128, 0));
                g.drawLine(getX(0), getY(0), getX(1), getY(0));
                g.drawLine(getX(1), getY(0), getX(1), getY(1));
                g.drawLine(getX(1), getY(1), getX(0), getY(1));
                g.drawLine(getX(0), getY(1), getX(0), getY(0));
            }
        }

        DrawerPanel panel;

        public Drawer() {
            super();

            panel = new DrawerPanel();
            getContentPane().add(panel);

            setSize(size, size);
            setTitle("Visualizer tool for problem CirclesSeparation");

            addKeyListener(new DrawerKeyListener(this));
            setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

            setResizable(false);
            setVisible(true);

            minX = 0.0;
            maxX = 1.0;
            minY = 0.0;
            maxY = 1.0;

            for (int i=0; i < fx.length; i++) {
                minX = Math.min(minX, fx[i] - r[i]);
                maxX = Math.max(maxX, fx[i] + r[i]);
                minY = Math.min(minY, fy[i] - r[i]);
                maxY = Math.max(maxY, fy[i] + r[i]);
            }

            scale = (size - 2 * PADDING) / Math.max(maxX - minX, maxY - minY);
        }
    }

    public double runTest() {
        solution = null;

        try {
            solution = Runtime.getRuntime().exec(execCommand);
        } catch (Exception e) {
            System.err.println("ERROR: Unable to execute your solution using the provided command: "
                    + execCommand + ".");
            return -1.0;
        }

        BufferedReader reader = new BufferedReader(new InputStreamReader(solution.getInputStream()));
        PrintWriter writer = new PrintWriter(solution.getOutputStream());
        new ErrorStreamRedirector(solution.getErrorStream()).start();

        Random rnd = null;
        try {
            rnd = SecureRandom.getInstance("SHA1PRNG");
        } catch (Exception e) {
            System.err.println("ERROR: unable to generate test case.");
            System.exit(1);
        }

        rnd.setSeed(seed);

        double t = rnd.nextDouble();
        N = MIN_N + (int)Math.floor((MAX_N - MIN_N + 1) * t * t);
        double from = Math.sqrt(MIN_R_FACTOR / (double)N);
        double to = Math.sqrt(MAX_R_FACTOR / (double)N);
        double maxR = from + rnd.nextDouble() * (to - from);

        x = new double[N];
        y = new double[N];
        r = new double[N];
        m = new double[N];
        for (int i=0; i<N; i++) {
            x[i] = rnd.nextDouble();
            y[i] = rnd.nextDouble();
            r[i] = maxR * rnd.nextDouble();
            m[i] = rnd.nextDouble();
        }

        writer.println(N);
        for (int i=0; i < N; i++) {
            writer.println(x[i]);
        }
        for (int i=0; i < N; i++) {
            writer.println(y[i]);
        }
        for (int i=0; i < N; i++) {
            writer.println(r[i]);
        }
        for (int i=0; i < N; i++) {
            writer.println(m[i]);
        }
        writer.flush();

        fx = new double[N];
        fy = new double[N];
        try {
            for (int i=0; i < N; i++) {
                fx[i] = Double.parseDouble(reader.readLine());
                fy[i] = Double.parseDouble(reader.readLine());
            }
        } catch (Exception e) {
            System.err.println("ERROR: unable to parse your return value.");
            e.printStackTrace();
            return -1.0;
        }

        stopSolution();

        for (int i=0; i < N; i++) {
            if (fx[i] < -MAX_COORD || fx[i] > MAX_COORD) {
                System.err.println("ERROR: the final X coordinate of " + i + "-th circle (0-based) must be in -100..100. Your return value = " + fx[i] + ".");
                return -1.0;
            }
            if (fy[i] < -MAX_COORD || fy[i] > MAX_COORD) {
                System.err.println("ERROR: the final Y coordinate of " + i + "-th circle (0-based) must be in -100..100. Your return value = " + fy[i] + ".");
                return -1.0;
            }
        }

        for (int i=0; i < N; i++) {
            for (int j=i+1; j < N; j++) {
                if ((fx[i] - fx[j]) * (fx[i] - fx[j]) + (fy[i] - fy[j]) * (fy[i] - fy[j]) < (r[i] + r[j]) * (r[i] + r[j])) {
                    System.err.println("ERROR: in your solution, circles " + i + " and " + j + " (0-based) overlap.");
                    return -1.0;
                }
            }
        }

        double score = 0.0;
        for (int i=0; i < N; i++) {
            score += m[i] * Math.sqrt((x[i] - fx[i]) * (x[i] - fx[i]) + (y[i] - fy[i]) * (y[i] - fy[i]));
        }

        if (vis) {
            drawer = new Drawer();
        }

        return score;
    }

    public static void stopSolution() {
        if (solution != null) {
            try {
                solution.destroy();
            } catch (Exception e) {
                // do nothing
            }
        }
    }

    public static void main(String[] args) {
        for (int i = 0; i < args.length; i++)
            if (args[i].equals("-exec")) {
                execCommand = args[++i];
            } else if (args[i].equals("-seed")) {
                seed = Long.parseLong(args[++i]);
            } else if (args[i].equals("-novis")) {
                vis = false;
            } else if (args[i].equals("-sz")) {
                size = Integer.parseInt(args[++i]);
            } else {
                System.out.println("WARNING: unknown argument " + args[i] + ".");
            }

        if (execCommand == null) {
            System.err.println("ERROR: You did not provide the command to execute your solution." +
                    " Please use -exec <command> for this.");
            System.exit(1);
        }

        CirclesSeparationVis vis = new CirclesSeparationVis();
        try {
            double score = vis.runTest();
            System.out.println("Score  = " + score);
        } catch (RuntimeException e) {
            System.err.println("ERROR: Unexpected error while running your test case.");
            e.printStackTrace();
            CirclesSeparationVis.stopSolution();
        }
    }
}

class ErrorStreamRedirector extends Thread {
    public BufferedReader reader;

    public ErrorStreamRedirector(InputStream is) {
        reader = new BufferedReader(new InputStreamReader(is));
    }

    public void run() {
        while (true) {
            String s;
            try {
                s = reader.readLine();
            } catch (Exception e) {
                // e.printStackTrace();
                return;
            }
            if (s == null) {
                break;
            }
            System.out.println(s);
        }
    }
}