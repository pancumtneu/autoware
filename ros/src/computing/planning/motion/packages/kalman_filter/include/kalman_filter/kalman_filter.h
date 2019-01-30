#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/LU>

class KalmanFilter {
public:
  KalmanFilter();
  KalmanFilter(const Eigen::MatrixXd &x, const Eigen::MatrixXd &A,
               const Eigen::MatrixXd &B, const Eigen::MatrixXd &C,
               const Eigen::MatrixXd &Q, const Eigen::MatrixXd &R,
               const Eigen::MatrixXd &P);
  ~KalmanFilter();

  void init(const Eigen::MatrixXd &x, const Eigen::MatrixXd &A,
            const Eigen::MatrixXd &B, const Eigen::MatrixXd &C,
            const Eigen::MatrixXd &Q, const Eigen::MatrixXd &R,
            const Eigen::MatrixXd &P);
  void init(const Eigen::MatrixXd &x, const Eigen::MatrixXd &P0);
  void setA(const Eigen::MatrixXd &A);
  void setB(const Eigen::MatrixXd &B);
  void setC(const Eigen::MatrixXd &C);
  void setQ(const Eigen::MatrixXd &Q);
  void setR(const Eigen::MatrixXd &R);
  void getX(Eigen::MatrixXd &x);
  void getP(Eigen::MatrixXd &P);

  void predictEKF(const Eigen::MatrixXd &x_next, const Eigen::MatrixXd &A,
                  const Eigen::MatrixXd &Q);
  void predictEKF(const Eigen::MatrixXd &x_next, const Eigen::MatrixXd &A);
  void predict(const Eigen::MatrixXd &u, const Eigen::MatrixXd &A,
               const Eigen::MatrixXd &B, const Eigen::MatrixXd &Q);
  void predict(const Eigen::MatrixXd &u);

  void updateEKF(const Eigen::MatrixXd &y, const Eigen::MatrixXd &y_pred,
                 const Eigen::MatrixXd &C, const Eigen::MatrixXd &R);
  void updateEKF(const Eigen::MatrixXd &y, const Eigen::MatrixXd &y_pred,
                 const Eigen::MatrixXd &C);
  void update(const Eigen::MatrixXd &y, const Eigen::MatrixXd &C,
              const Eigen::MatrixXd &R);
  void update(const Eigen::MatrixXd &y);

private:
  Eigen::MatrixXd x_;
  Eigen::MatrixXd A_;
  Eigen::MatrixXd B_;
  Eigen::MatrixXd C_;
  Eigen::MatrixXd Q_;
  Eigen::MatrixXd R_;
  Eigen::MatrixXd P_;
};

KalmanFilter::KalmanFilter() {}
KalmanFilter::KalmanFilter(const Eigen::MatrixXd &x, const Eigen::MatrixXd &A,
                           const Eigen::MatrixXd &B, const Eigen::MatrixXd &C,
                           const Eigen::MatrixXd &Q, const Eigen::MatrixXd &R,
                           const Eigen::MatrixXd &P) {
  init(x, A, B, C, Q, R, P);
}
KalmanFilter::~KalmanFilter() {}
void KalmanFilter::init(const Eigen::MatrixXd &x, const Eigen::MatrixXd &A,
                        const Eigen::MatrixXd &B, const Eigen::MatrixXd &C,
                        const Eigen::MatrixXd &Q, const Eigen::MatrixXd &R,
                        const Eigen::MatrixXd &P) {
  x_ = x;
  A_ = A;
  B_ = B;
  C_ = C;
  Q_ = Q;
  R_ = R;
  P_ = P;
}
void KalmanFilter::init(const Eigen::MatrixXd &x, const Eigen::MatrixXd &P0) {
  x_ = x;
  P_ = P0;
}

void KalmanFilter::setA(const Eigen::MatrixXd &A) { A_ = A; }
void KalmanFilter::setB(const Eigen::MatrixXd &B) { B_ = B; }
void KalmanFilter::setC(const Eigen::MatrixXd &C) { C_ = C; }
void KalmanFilter::setQ(const Eigen::MatrixXd &Q) { Q_ = Q; }
void KalmanFilter::setR(const Eigen::MatrixXd &R) { R_ = R; }
void KalmanFilter::getX(Eigen::MatrixXd &x) { x = x_; };
void KalmanFilter::getP(Eigen::MatrixXd &P) { P = P_; };

void KalmanFilter::predictEKF(const Eigen::MatrixXd &x_next,
                              const Eigen::MatrixXd &A,
                              const Eigen::MatrixXd &Q) {
  x_ = x_next;
  P_ = A * P_ * A.transpose() + Q;
}
void KalmanFilter::predictEKF(const Eigen::MatrixXd &x_next,
                              const Eigen::MatrixXd &A) {
  predictEKF(x_next, A, Q_);
}

void KalmanFilter::predict(const Eigen::MatrixXd &u, const Eigen::MatrixXd &A,
                           const Eigen::MatrixXd &B, const Eigen::MatrixXd &Q) {
  const Eigen::MatrixXd x_next = A * x_ + B * u;
  predictEKF(x_next, A, Q);
}
void KalmanFilter::predict(const Eigen::MatrixXd &u) { predict(u, A_, B_, Q_); }

void KalmanFilter::updateEKF(const Eigen::MatrixXd &y,
                             const Eigen::MatrixXd &y_pred,
                             const Eigen::MatrixXd &C,
                             const Eigen::MatrixXd &R) {
  const Eigen::MatrixXd S = R + C * P_ * C.transpose();
  const Eigen::MatrixXd K = P_ * C.transpose() * S.inverse();
  x_ = x_ + K * (y - y_pred);
  const int dim = P_.cols();
  const Eigen::MatrixXd I = Eigen::MatrixXd::Identity(dim, dim);
  P_ = (I - K * C) * P_;
}
void KalmanFilter::updateEKF(const Eigen::MatrixXd &y,
                             const Eigen::MatrixXd &y_pred,
                             const Eigen::MatrixXd &C) {
  updateEKF(y, y_pred, C, R_);
}
void KalmanFilter::update(const Eigen::MatrixXd &y, const Eigen::MatrixXd &C,
                          const Eigen::MatrixXd &R) {
  const Eigen::MatrixXd y_pred = C * x_;
  updateEKF(y, y_pred, C, R);
}
void KalmanFilter::update(const Eigen::MatrixXd &y) { update(y, C_, R_); }