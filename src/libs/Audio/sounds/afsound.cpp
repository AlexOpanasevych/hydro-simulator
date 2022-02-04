#include "afsound.h"

AFSound::AFSound(QObject *parent) : Sound(parent)
{

}

void AFSound::setForm(std::function<double (double)> formFunc)
{
    m_formFunction = formFunc;
}

double AFSound::signal(qint64 index)
{
    static int ind = 0;
    if (m_formFunction != nullptr)
    {
        return m_formFunction(static_cast<double>(ind++) / sampleRate());
    }
    return 0;
}
