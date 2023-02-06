#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_SCALABLEOCTAVES_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_SCALABLEOCTAVES_H_

class ScalableOctaves {
 public:
  void SetScale(double scale);

  double GetXScale() const;

  double GetYScale() const;

  double GetZScale() const;

  void SetXScale(double scaleX);

  void SetYScale(double scaleY);

  void SetZScale(double scaleZ);

 protected:
  double xScale = 1.0;
  double yScale = 1.0;
  double zScale = 1.0;
};

#endif //EXT_VANILLAGENERATOR_LIB_VANILLA_SCALABLEOCTAVES_H_
