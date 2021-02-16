#include "imagetobase64.h"

QByteArray imageToBase64(const QImage &image)
{
    QByteArray dataArray;
    QBuffer buff(&dataArray);
    image.save(&buff, "PNG", 20);
    QByteArray hexed = dataArray.toBase64();
    buff.close();
    return hexed;
}
