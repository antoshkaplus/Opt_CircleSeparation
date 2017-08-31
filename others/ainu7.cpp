#include <stdio.h>
#include <math.h>
#include <map>
#include <sys/time.h>
#include <vector>
#include <queue>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <sstream>
using namespace std;

class CRandomMother {
public:
    void RandomInit(int seed);
    double Random();
    uint32_t BRandom();
    CRandomMother(int seed) {
        RandomInit(seed);}
protected:
    uint32_t x[5];
};

uint32_t CRandomMother::BRandom() {
    uint64_t sum;
    sum = (uint64_t)2111111111UL * (uint64_t)x[3] +
          (uint64_t)1492 * (uint64_t)(x[2]) +
          (uint64_t)1776 * (uint64_t)(x[1]) +
          (uint64_t)5115 * (uint64_t)(x[0]) +
          (uint64_t)x[4];
    x[3] = x[2];  x[2] = x[1];  x[1] = x[0];
    x[4] = (uint32_t)(sum >> 32);                  // Carry
    x[0] = (uint32_t)sum;                          // Low 32 bits of sum
    return x[0];
}


void CRandomMother::RandomInit (int seed) {
    int i;
    uint32_t s = seed;
    for (i = 0; i < 5; i++) {
        s = s * 29943829 - 1;
        x[i] = s;
    }
    for (i=0; i<19; i++) BRandom();
}

CRandomMother* _random;
double time_multiplier = 1.0;
double ultimate_time_limit = 4.5;
double total_time_limit = 9.3;
double emergency_time_limit = 9.9;
double time_limit_margin = 0.05;
double time_start = 0.0;

double getTime()
{
    timeval tv;
    gettimeofday(&tv, 0);
    return (tv.tv_sec + tv.tv_usec * 0.000001 - time_start) * time_multiplier;
}

double initGetTime()
{
    timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec * 0.000001;
}

class CirclesSeparation2
{
public:
    vector<float> trivialSolution(vector<float> x, vector<float> y, vector<float> r, vector<float> m) {
        int N = x.size();
        float s = 0;
        float t = 0;
        int step = (int)(sqrt(N) * 1.2);
        float mmax = 0.0;
        for (int i=0; i<N; i++)
            mmax = max(mmax, r[i]);
        vector<int> order(N);
        for (int i=0; i<N; i++)
            order[i] = i;

        for (int i=0; i<N; i++)
            for (int j=i+1; j<N; j++)
                if (y[i] > y[j]) {
                    swap(x[i], x[j]);
                    swap(y[i], y[j]);
                    swap(r[i], r[j]);
                    swap(m[i], m[j]);
                    swap(order[i], order[j]);
                }
        for (int i=0; i<N; i+=step) {
            int j = min(i+step, N);
            for (int k=i; k<j; k++)
                for (int l=k+1; l<j; l++)
                    if (x[k] > x[l]) {
                        swap(x[k], x[l]);
                        swap(y[k], y[l]);
                        swap(r[k], r[l]);
                        swap(m[k], m[l]);
                        swap(order[k], order[l]);
                    }
        }


        float here_max = 0.0;
        vector<float> res(N*2);
        vector<int> end;
        for (int i=0; i<N; i++) {
            if (i % step == 0) {
                if (i) {
                    t += mmax + here_max;
                    end.push_back(i-1);
                }
                s = 0;
                here_max = 0.0;
            } else {
                s += r[i] + r[i-1];
            }
            res[order[i]*2] = s;
            res[order[i]*2+1] = t;
            here_max = max(here_max, r[i]);
        }
        if (end[end.size()-1] != N-1)
            end.push_back(N-1);


        // x-adjust
        int start = 0;
        for (int i=0; i<end.size(); i++) {
            float x_sum = 0.0;
            for (int j=start; j<=end[i]; j++)
                x_sum += res[order[j]*2];
            for (int j=start; j<=end[i]; j++)
                res[order[j]*2] -= x_sum / (end[i]-start+1) - 0.5;
            start = end[i] + 1;
        }

        // y-adjust
        for (int i=0; i+1<end.size(); i++) {
            int s1 = (i==0)?0:end[i-1]+1;
            int f1 = end[i];
            int s2 = end[i] + 1;
            int f2 = end[i+1];
            float poss_diff = res[order[s2]*2+1] - res[order[s1]*2+1];

            float min_diff = 0.0;
            for (int j=0; j<30; j++) {
                float now_diff = (min_diff + poss_diff) / 2;
                int poss = 1;
                for (int k=s1; k<=f1; k++)
                    for (int l=s2; l<=f2; l++) {
                        float x_dist = res[order[k]*2] - res[order[l]*2];
                        float y_dist = now_diff;
                        float dist_sq = x_dist * x_dist + y_dist * y_dist;
                        if (dist_sq < (r[k]+r[l])*(r[k]+r[l]))
                            poss = 0;
                    }
                if (poss) poss_diff = now_diff; else min_diff = now_diff;
            }
            for (int j=s2; j<=f2; j++)
                res[order[j]*2+1] = res[order[s1]*2+1] + poss_diff;
        }

        // remaining adjust
        float x_sum = 0.0, y_sum = 0.0;
        for (int i=0; i<N; i++) {
            x_sum += res[i*2];
            y_sum += res[i*2+1];
        }
        for (int i=0; i<N; i++) {
            res[i*2] -= x_sum / N - 0.5;
            res[i*2+1] -= y_sum / N - 0.5;
        }

        return res;
    };

    vector<float> minimumWork2(vector<float> x, vector<float> y, vector<float> r, vector<float> m, vector<float> res, double time_limit, int limit) {

        const int max_n = 500;
        float xx[max_n], yy[max_n];
        vector<int> order;
        int n = x.size();
        float max_r = 0.0;
//    fprintf(stderr, "n = %d\n", n);
        float area = 0.0;
        for (int i=0; i<n; i++) {
            order.push_back(i);
            area += r[i] * r[i] * 3.141592653589793238;
            max_r = max(max_r, r[i]);
            xx[i] = res[i*2];
            yy[i] = res[i*2+1];
        }
//    fprintf(stderr, "area = %f\n", area);
        for (int i=0; i<n; i++)
            for (int j=i+1; j<n; j++)
                if (res[order[i]*2] > res[order[j]*2])
                    swap(order[i], order[j]);
        float eff_rev_m[max_n], real_mmax[max_n], real_dist[max_n];
        for (int i=0; i<n; i++) {
            eff_rev_m[i] = 1.0 / max(0.01, pow(m[i], 1.0)); // pow 1.3 : good for big n, bad for small n
            real_dist[i] = sqrt((xx[i]-x[i])*(xx[i]-x[i])+(yy[i]-y[i])*(yy[i]-y[i]));
        }
        float um = 1.0, target_um = 0.1, switch_um = 2.0;

        unsigned int tr = 0;

        float mul = sqrt(area / 8.0) * max(1.0, 1.25 - (n-50)/40.0*0.14);
//    fprintf(stderr, "%f\n", mul);
//    double score_idx = pow(n, 0.7) * pow(area, 1.3) / 170;
//    mul *= pow(score_idx, 0.15);
//    if (n > 300) mul *= 1.2;
        int left_right = n / 4;

        int lll[max_n], step[max_n];
        for (int i=0; i<n; i++) {
            lll[i] = max(0, i - left_right);
            step[i] = min(n-1, i + left_right) - lll[i] + 1;
        }
        int tr_map[64];
        for (int i=0; i<64; i++)
            tr_map[i] = (i%5 < 2);

#ifdef LOCAL
        for (tr=0; tr<limit; tr++) {
      if ((tr&((1<<13)-1)) == 0) {
        um = pow((limit - tr) / (float)limit, 1.7f);
#else
        for (tr=0; ; tr++) {
            if ((tr&((1<<13)-1)) == 0) {
                float phase = getTime() / time_limit;
                if (phase > 1.0f) break;
                um = pow(1.0f - phase, 1.7f);
#endif
                target_um = um * 0.1f * mul;
                switch_um = um * 2.0f;
                for (int i=0; i<n; i++)
                    real_mmax[i] = target_um * eff_rev_m[i];
            }
//      uint32_t rnd = _random->BRandom();
            uint32_t rnd2 = _random->BRandom();
            if (tr_map[tr&63u]) {
                // swapping
                // this is the start of chain, so made faster sacrificing some accuracy.
                static int ot = 0;
                ot ++;
                if (ot == n) ot = 0;
                int order_target1 = ot;
                // order_target1 = rnd % n;

                int ll, ste;
//        if (rnd2 & (1<<16)) { // deleting this line will give +0.058
                ll = lll[order_target1];
                ste = step[order_target1];
//        }
                int order_target2 = ((int)((rnd2&((1u<<15)-1u)) * ste) >> 15) + ll;
                int order_target3 = ((int)(((rnd2>>15)&((1u<<15)-1u)) * ste) >> 15) + ll;
                int target1 = order[order_target1];
                int target2 = order[order_target2];
                int target3 = order[order_target3];


                float dr12 = (xx[target1]-xx[target2])*(xx[target1]-xx[target2])+(yy[target1]-yy[target2])*(yy[target1]-yy[target2]);
                float dr13 = (xx[target1]-xx[target3])*(xx[target1]-xx[target3])+(yy[target1]-yy[target3])*(yy[target1]-yy[target3]);
                float dr23 = (xx[target2]-xx[target3])*(xx[target2]-xx[target3])+(yy[target2]-yy[target3])*(yy[target2]-yy[target3]);
                if (dr13 < dr12 && dr13 < dr23) order_target2 = order_target3, target2 = target3, dr12 = dr13;
                if (dr23 < dr12 && dr23 < dr13) order_target1 = order_target3, target1 = target3, dr12 = dr23;
                if (target1 == target2) continue;
                if (r[target1] > r[target2]) {
                    swap(order_target1, order_target2);
                    swap(target1, target2);
                }
                // score check
                float score = 0.0f;
                float dist12 = sqrt((x[target1] - xx[target2]) * (x[target1] - xx[target2]) + (y[target1] - yy[target2]) * (y[target1] - yy[target2]));
                float dist21 = sqrt((x[target2] - xx[target1]) * (x[target2] - xx[target1]) + (y[target2] - yy[target1]) * (y[target2] - yy[target1]));
                score += (real_dist[target1] - dist12) * m[target1];
                score += (real_dist[target2] - dist21) * m[target2];
                float new_x = xx[target2];
                float new_y = yy[target2];
                if (dr12 < (r[target2] * 2.0f) * (r[target2] * 2.0f)) {
                    float pr = (sqrt(dr12) - r[target1] - r[target2]) * m[target1];
                    score += pr;
                    if (score < 0.0f) continue;
                } else {
                    float pr = (r[target2]-r[target1]) * m[target1];
                    score += pr;

                    pr = r[target1] * m[target2] * switch_um; // pr = (r[target1] * m[target1] + r[target2] * m[target2]) * um; might be better

                    if (score < -pr) continue;
                    if (score < 0.0f && -score * (65536.f*65536.f) > rnd2 * pr) continue; // finally SA helps? T.T
/*          double dx = xx[target2] - x[target1];
          double dy = yy[target2] - y[target1];
          double target_dist = max(sqrt(dx*dx + dy*dy), r[target2] - r[target1]);
          new_x = x[target1] + dx * target_dist / sqrt(dx*dx + dy*dy);
          new_y = y[target1] + dy * target_dist / sqrt(dx*dx + dy*dy);*/
                }
                // now, check whether target2 can be moved to the place of target1.
                int poss = 1;
                float eff_r = r[target2];
                float x_left = xx[target1] - (max_r + eff_r);
                float x_right = xx[target1] + (max_r + eff_r);
                int st = order_target1+1;
//        while (st+2 < n && xx[order[st+2]] < x_left) st += 3;
                for (int i=st; i<n; i++) {
                    if (i == order_target2) continue;
                    int l = order[i];
                    float x_dist = xx[l] - xx[target1];
                    float y_dist = yy[l] - yy[target1];
                    float dist_sq = x_dist * x_dist + y_dist * y_dist;
                    if (xx[l] > x_right) break;
                    if (dist_sq < (eff_r+r[l])*(eff_r+r[l])) {
                        poss = 0;
                        break;
                    }
                }
                if (poss) {
                    int st = order_target1-1;
//          while (st-2 >= 0 && xx[order[st-2]] > x_right) st -= 3;
                    for (int i=st; i>=0; i--) {
                        if (i == order_target2) continue;
                        int l = order[i];
                        float x_dist = xx[l] - xx[target1];
                        float y_dist = yy[l] - yy[target1];
                        float dist_sq = x_dist * x_dist + y_dist * y_dist;
                        if (xx[l] < x_left) break;
                        if (dist_sq < (eff_r+r[l])*(eff_r+r[l])) {
                            poss = 0;
                            break;
                        }
                    }
                }

                if (poss) {
//        static int q = 0; q++;
//        if (q%1000 == 0) fprintf(stderr, "%d %d %f %d %d %f %f %f %f\n", q, tr, score, order_target1, order_target2, r[order[order_target1]], r[order[order_target2]], dist12, dist21);
                    swap(xx[target1], xx[target2]);
                    swap(yy[target1], yy[target2]);
                    swap(order[order_target1], order[order_target2]);

/*          if (xx[target1] < new_x) {
            while (order_target2+1 < n && new_x > xx[order[order_target2+1]]) {
              order[order_target2] = order[order_target2+1];
              order_target2 ++;
            }
          } else {
            while (order_target2-1 >= 0 && xx[order[order_target2-1]] > new_x) {
              order[order_target2] = order[order_target2-1];
              order_target2 --;
            }
          }
          order[order_target2] = target1;

          xx[target1] = new_x;
          yy[target1] = new_y;*/

                    real_dist[target1] = dist12;
                    real_dist[target2] = dist21;
                }
                continue;
            }
            static int order_target = 0;
            order_target ++;
            if (order_target == n) order_target = 0;

            int target = order[order_target];
            float mmax = real_mmax[target];
            int hyper = (rnd2&((1u<<16)+(1u<<17))) == 0;
            if (hyper) mmax *= 6.0f; //mmax = sqrt(target_um) * eff_rev_m[target]; - nearly same?
            float xxx = (int32_t)(((rnd2 & 65535) - 32768)) / (65536.f/2.f);
            float yyy = (int32_t)rnd2 / (65536.f*65536.f/2.f);
            float x_move = mmax * xxx;
            float y_move = mmax * yyy;
            if (hyper || (rnd2&31u) < 13) {
                // bias
                if (x_move * (x[target] - xx[target]) < y_move * (yy[target] - y[target])) {
                    x_move = -x_move;
                    y_move = -y_move;
                }
            }
            float new_x = xx[target] + x_move;
            float new_y = yy[target] + y_move;

            if (hyper)
            {
                float score = real_dist[target] * real_dist[target];
                float new_score = (x[target] - new_x) * (x[target] - new_x) + (y[target] - new_y) * (y[target] - new_y);

//        score *= m[target];
                if (score < new_score) continue;
            }

/*      if (hyper)
      {
        double score = sqrt((x[target] - xx[target]) * (x[target] - xx[target]) + (y[target] - yy[target]) * (y[target] - yy[target]));
        score -= sqrt((x[target] - new_x) * (x[target] - new_x) + (y[target] - new_y) * (y[target] - new_y));
        double pr = target_um * 0.1;
        if (score < 0 && -score / pr > _random->Random()) continue; // finally SA helps? T.T
      }*/
            int poss = 1;
            float eff_r = r[target];
            float x_left = new_x - (max_r + eff_r);
            float x_right = new_x + (max_r + eff_r);
            int st = order_target+1;
            while (st+2 < n && xx[order[st+2]] < x_left) st += 3;
            for (int i=st; i<n; i++) {
                int l = order[i];
                float x_dist = xx[l] - new_x;
                float y_dist = yy[l] - new_y;
                float dist_sq = x_dist * x_dist + y_dist * y_dist;
                if (xx[l] > x_right) break;
                if (dist_sq < (eff_r+r[l])*(eff_r+r[l])) {
                    poss = 0;
                    break;
                }
            }
            if (poss) {
                int st = order_target-1;
                while (st-2 >= 0 && xx[order[st-2]] > x_right) st -= 3;
                for (int i=st; i>=0; i--) {
                    int l = order[i];
                    float x_dist = xx[l] - new_x;
                    float y_dist = yy[l] - new_y;
                    float dist_sq = x_dist * x_dist + y_dist * y_dist;
                    if (xx[l] < x_left) break;
                    if (dist_sq < (eff_r+r[l])*(eff_r+r[l])) {
                        poss = 0;
                        break;
                    }
                }
            }

            if (poss) {
                xx[target] = new_x;
                yy[target] = new_y;
                if (x_move > 0) {
                    while (order_target+1 < n && new_x > xx[order[order_target+1]]) {
                        order[order_target] = order[order_target+1];
                        order_target ++;
                    }
                } else {
                    while (order_target-1 >= 0 && xx[order[order_target-1]] > new_x) {
                        order[order_target] = order[order_target-1];
                        order_target --;
                    }
                }
                order[order_target] = target;
                real_dist[target] = sqrt((x[target] - new_x) * (x[target] - new_x) + (y[target] - new_y) * (y[target] - new_y));

                order_target ++;
                if (order_target == n) order_target = 0;
            }
        }
/*  for (int i=0; i<n; i++)
      for (int j=i+1; j<n; j++)
        for (int k=j+1; k<n; k++) {
          int t1 = i;
          int t2 = j;
          int t3 = k;
          if (r[t1] > r[t2]) swap(t1, t2);
          if (r[t1] > r[t3]) swap(t1, t3);
          if (r[t2] > r[t3]) swap(t2, t3);
          // r[t1] < r[t2] < r[t3]
          double prev_dist = 0.0;
          prev_dist += sqrt((x[t1]-xx[t1])*(x[t1]-xx[t1])+(y[t1]-yy[t1])*(y[t1]-yy[t1])) * m[t1];
          prev_dist += sqrt((x[t2]-xx[t2])*(x[t2]-xx[t2])+(y[t2]-yy[t2])*(y[t2]-yy[t2])) * m[t2];
          prev_dist += sqrt((x[t3]-xx[t3])*(x[t3]-xx[t3])+(y[t3]-yy[t3])*(y[t3]-yy[t3])) * m[t3];
          double new_dist = 0.0;
          new_dist += sqrt((x[t1]-xx[t2])*(x[t1]-xx[t2])+(y[t1]-yy[t2])*(y[t1]-yy[t2])) * m[t1];
          new_dist += sqrt((x[t2]-xx[t3])*(x[t2]-xx[t3])+(y[t2]-yy[t3])*(y[t2]-yy[t3])) * m[t2];
          new_dist += sqrt((x[t3]-xx[t1])*(x[t3]-xx[t1])+(y[t3]-yy[t1])*(y[t3]-yy[t1])) * m[t3];
          if (prev_dist < new_dist) continue;

          // t1->t2, t2->t3, t3->t1
          // check t3->t1 possible?
          int ok = 1;
          for (int l=0; l<n; l++)
            if (l!=i && l!=j && l!=k) {
              if ((xx[l]-xx[t1])*(xx[l]-xx[t1])+(yy[l]-yy[t1])*(yy[l]-yy[t1]) < (r[l]+r[t3]+eps)*(r[l]+r[t3]+eps)) {
                ok = 0;
                break;
              }
            }
          if (!ok) continue;
          fprintf(stderr, "%d %d %d %f %f\n", t1, t2, t3, prev_dist, new_dist);
        }*/

        for (int i=0; i<n; i++) {
            res[i*2] = xx[i];
            res[i*2+1] = yy[i];
        }

        fprintf(stderr, "tr = %d\n", tr);
#ifndef LOCAL
        // joojak
        float rr = 0.0;
        for (int i=0; i<n; i++) {
            float dx = res[i*2] - x[i];
            float dy = res[i*2+1] - y[i];
            rr += sqrt(dx * dx + dy * dy) * m[i];
        }
        float add = rr / 1.00 - rr;
        fprintf(stderr, "before: %f\n", rr);

        {
            float x_min = 1.0, idx = 0;
            for (int i=0; i<n; i++)
                if (res[i*2] - r[i] < x_min) { x_min = res[i*2] - r[i]; idx = i; }
            if (add / m[idx] < 90) {
                float ll = 0.0; rr = 95.0;
                for (int i=0; i<100; i++) {
                    float now = (ll + rr) / 2;
                    float sq1 = (res[idx*2] - x[idx]) * (res[idx*2] - x[idx]) + (res[idx*2+1] - y[idx]) * (res[idx*2+1] - y[idx]);
                    float sq2 = (res[idx*2] - now - x[idx]) * (res[idx*2] - now - x[idx]) + (res[idx*2+1] - y[idx]) * (res[idx*2+1] - y[idx]);
                    float diff = sqrt(sq2) - sqrt(sq1);
                    if (diff < add / m[idx]) ll = now; else rr = now;
                }
                res[idx*2] -= ll;
                fprintf(stderr, "joojak 1\n");
                return res;
            }
        }
        {
            float x_max = -1.0, idx = 0;
            for (int i=0; i<n; i++)
                if (res[i*2] + r[i] > x_max) { x_max = res[i*2] + r[i]; idx = i; }
            if (add / m[idx] < 90) {
                float ll = 0.0; rr = 95.0;
                for (int i=0; i<100; i++) {
                    float now = (ll + rr) / 2;
                    float sq1 = (res[idx*2] - x[idx]) * (res[idx*2] - x[idx]) + (res[idx*2+1] - y[idx]) * (res[idx*2+1] - y[idx]);
                    float sq2 = (res[idx*2] + now - x[idx]) * (res[idx*2] + now - x[idx]) + (res[idx*2+1] - y[idx]) * (res[idx*2+1] - y[idx]);
                    float diff = sqrt(sq2) - sqrt(sq1);
                    if (diff < add / m[idx]) ll = now; else rr = now;
                }
                res[idx*2] += ll;
                fprintf(stderr, "joojak 2\n");
                return res;
            }
        }
#endif

        return res;
    }

    vector<double> minimumWork(vector<double> x_, vector<double> y_, vector<double> r_, vector<double> m_) {
        double total_time = emergency_time_limit;

        time_start = initGetTime();
//    _random = new CRandomMother(time(0));
        _random = new CRandomMother(((long long)(time_start*1000))&((long long)(1<<31)-1));

        const double eps = 2e-6;
        int N = x_.size();
        vector<float> x(N), y(N), r(N), m(N);
        for (int i=0; i<N; i++) {
            x[i] = x_[i];
            y[i] = y_[i];
            r[i] = r_[i] + eps;
            m[i] = m_[i];
        }
        vector<float> trivial = trivialSolution(x, y, r, m);
        vector<float> res = trivial;
        int trial = 1;
        if (N <= 133) trial = 2;
        if (N <= 92) trial = 3;
        double mmin = 999999999.99;
        for (int i=0; i<trial; i++) { // todo: iterate?
            vector<float> now = minimumWork2(x, y, r, m, trivial, getTime() + (total_time - getTime()) / (trial - i), 52000000 / trial);
            float rr = 0.0;
            for (int i=0; i<x.size(); i++) {
                float dx = now[i*2] - x[i];
                float dy = now[i*2+1] - y[i];
                rr += sqrt(dx * dx + dy * dy) * m[i];
            }
            fprintf(stderr, "%f\n", rr);
            if (rr < mmin) { mmin = rr; res = now; }
        }
        vector<double> real_res;
        for (int i=0; i<N*2; i++)
            real_res.push_back(res[i]);
        return real_res;
    }

};


class CirclesSeparation {
public:
    vector<double> minimumWork(vector<double> x_, vector<double> y_, vector<double> r_, vector<double> m_);
};

vector<double> CirclesSeparation::minimumWork(vector<double> x_, vector<double> y_, vector<double> r_, vector<double> m_) {
    CirclesSeparation2 cs;
    return cs.minimumWork(x_, y_, r_, m_);
}