#include "util.h"

// constructs a plane from a collection of points
// such that the summed squared distance to all points is minimized
// https://www.ilikebigbits.com/2015_03_04_plane_from_points.html
glm::vec3 PlaneNormalFromPoints(const std::vector<glm::vec3> &points) {
    if (points.size() < 3) {
        return glm::vec3(0);
    }

    // compute centroid
    glm::vec3 sum(0);
    for (const auto &p : points) {
        sum += p;
    }
    const glm::vec3 centroid = sum / static_cast<float>(points.size());

    // compute full 3x3 covariance matrix, excluding symmetries
    float xx = 0; float xy = 0; float xz = 0;
    float yy = 0; float yz = 0; float zz = 0;
    for (const auto &p : points) {
        const auto r = p - centroid;
        xx += r.x * r.x;
        xy += r.x * r.y;
        xz += r.x * r.z;
        yy += r.y * r.y;
        yz += r.y * r.z;
        zz += r.z * r.z;
    }

    const float det_x = yy * zz - yz * yz;
    const float det_y = xx * zz - xz * xz;
    const float det_z = xx * yy - xy * xy;
    const float det_max = std::max({det_x, det_y, det_z});
    if (det_max <= 0) {
        return glm::vec3(0);
    }

    // choose path with best conditioning
    if (det_max == det_x) {
        return glm::normalize(glm::vec3(
            det_x, xz * yz - xy * zz, xy * yz - xz * yy));
    } else if (det_max == det_y) {
        return glm::normalize(glm::vec3(
            xz * yz - xy * zz, det_y, xy * xz - yz * xx));
    } else {
        return glm::normalize(glm::vec3(
            xy * yz - xz * yy, xy * xz - yz * xx, det_z));
    }
}