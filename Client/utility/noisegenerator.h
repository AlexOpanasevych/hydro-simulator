#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H


class NoiseGenerator {
    double m_amplitude = 1;
public:
    NoiseGenerator();
    virtual double generate(int i) = 0;
    virtual ~NoiseGenerator();
    double amplitude() const;
    void setAmplitude(double amplitude);
};

#endif // NOISEGENERATOR_H
