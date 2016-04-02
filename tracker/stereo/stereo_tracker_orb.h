#ifndef TRACKER_STEREO_STEREO_TRACKER_ORB_
#define TRACKER_STEREO_STEREO_TRACKER_ORB_

#include "stereo_tracker_base.h"

#include <opencv2/core/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/cudafeatures2d.hpp>

namespace track {

class StereoTrackerORB : public StereoTrackerBase {
 public:
  StereoTrackerORB(size_t max_tracks, size_t max_xdiff, double max_epipolar_diff,
                   size_t max_disp, size_t max_disp_diff, int patch_size,
                   float scale_factor, int num_levels,
                   int maxdist_stereo, int maxdist_temp);
  void init(const cv::Mat& img_left, const cv::Mat& img_right) override;
  void track(const cv::Mat& img_left, const cv::Mat& img_right) override;
  int countFeatures() const override { return max_tracks_; }
  FeatureInfo featureLeft(int i) const override;
  FeatureInfo featureRight(int i) const override;
  void removeTrack(int i) override { age_[i] = -1; }
  bool IsAlive(int i) const override { return age_[i] > 0; }
  //int countActiveTracks() const override { return alive_cnt_; }
  int countActiveTracks() const override {
    int cnt = 0;
    for (int a : age_)
      if (a > 0)
        cnt++;
    return cnt;
  }

 private:
  void DrawMatches() const;
  void DrawStereo() const;
  void DrawFullTracks() const;
  void DrawKeypoints(const cv::Mat& img, const std::vector<cv::KeyPoint>& points,
                     std::string window_name) const;
  void ApplyEpipolarConstraint(const std::vector<cv::KeyPoint>& points_left,
                               const std::vector<cv::KeyPoint>& points_right,
                               const std::vector<std::vector<size_t>>& row_indices,
                               cv::Mat& mask) const;
  void ApplyTemporalConstraint(const std::vector<cv::KeyPoint>& points_curr, cv::Mat& mask) const;

  size_t max_tracks_;
  int max_xdiff_, max_ydiff_;
  double max_epipolar_diff_;
  int max_disp_, max_disp_diff_;
  int maxdist_temp_, maxdist_stereo_;
  cv::cuda::Stream cuda_stream_;
  cv::Ptr<cv::cuda::ORB> detector_;
  cv::Ptr<cv::cuda::DescriptorMatcher> matcher_;
  cv::cuda::GpuMat gpuimg_left_, gpuimg_right_;
  cv::cuda::GpuMat descriptors_lp_;

  cv::Mat img_lp_, img_rp_, img_lc_, img_rc_;
  std::vector<cv::KeyPoint> tracks_lp_, tracks_rp_, tracks_lc_, tracks_rc_;
  std::vector<int> age_;
  std::vector<int> alive_indices_;
  std::vector<bool> temp_matches_status_;
  int alive_cnt_ = 0;
  int img_rows_ = 0, img_cols_ = 0;
  bool verbose_ = false;
};

} // namespace track

#endif  // TRACKER_STEREO_STEREO_TRACKER_ORB_
