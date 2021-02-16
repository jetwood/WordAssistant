#ifndef IMAGETOBASE64_H
#define IMAGETOBASE64_H
#include <QImage>
#include <QBuffer>

/* the format of image file converted from a byte base to Base64
 * used by internet transfer.*/
QByteArray imageToBase64(const QImage &image);

#endif // IMAGETOBASE64_H
