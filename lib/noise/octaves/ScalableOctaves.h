#ifndef EXT_NOISELIB_LIB_VANILLA_SCALABLEOCTAVES_H_
#define EXT_NOISELIB_LIB_VANILLA_SCALABLEOCTAVES_H_

class ScalableOctaves {
 public:
  void setScale(double scale);

  double getXScale() const;

  double getYScale() const;

  double getZScale() const;

  void setXScale(double scaleX);

  void setYScale(double scaleY);

  void setZScale(double scaleZ);

 protected:
  double xScale = 1.0;
  double yScale = 1.0;
  double zScale = 1.0;
};

#endif //EXT_NOISELIB_LIB_VANILLA_SCALABLEOCTAVES_H_
