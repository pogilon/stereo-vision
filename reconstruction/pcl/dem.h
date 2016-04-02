#ifndef RECONSTRUCTION_BASE_DEM_
#define RECONSTRUCTION_BASE_DEM_

#include <Eigen/Core>
#include <pcl/common/common_headers.h>

namespace recon {

class DEM
{
 public:
  DEM(double x_start, double x_end, double z_start, double z_end, int x_cells, int z_cells, double elevation_thr);
  ~DEM();

  void update(const Eigen::Vector3d& point, double elevation);
  void update(const pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud, const std::vector<double>& elevations);
  //double getCell(int x_cell, int z_cell, Eigen::Vector3d& center);
  double getCell(int x_cell, int z_cell, const Eigen::Vector4d& plane, Eigen::Vector3d& center) const;
  int getSizeX() const { return x_cells_; }
  int getSizeZ() const { return z_cells_; }
  bool isCellVisible(int x_cell, int z_cell) const;

 private:
  double x_start_, x_end_, z_start_, z_end_;
  double width_;
  double length_;
  int x_cells_;
  int z_cells_;
  double cell_width_;
  double cell_length_;
  double elevation_thr_;

  std::vector<double> dem_;         // DEM - digital elevation map
  std::vector<bool> visibility_map_;
  std::vector<int> density_map_;
};

}

#endif
