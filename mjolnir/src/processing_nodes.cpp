#include "processing_nodes.h"

nodo_gris::nodo_gris(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(100, 100, 100);
    sid = "Gris" + sid;
}

void nodo_gris::procesar()
{
    if (!m_src.empty())
        cv::cvtColor(m_src, m_out, cv::COLOR_BGR2GRAY);
}

/********************************************/

nodo_blur::nodo_blur(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(100, 50, 190);
    sid = "blur" + sid;
    cv::namedWindow(sid);
    cv::createTrackbar("kernel_sz", sid, &kernel_sz, 23);
}

void nodo_blur::procesar()
{
    if (!m_src.empty())
    {
        kernel_sz = kernel_sz < 1 ? 1 : kernel_sz;
        cv::blur(m_src, m_out, cv::Size(kernel_sz, kernel_sz));
    }
};

/***********************************************/

nodo_canny::nodo_canny(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(230, 230, 230);
    sid = "Canny" + sid;
    cv::namedWindow(sid);
    cv::createTrackbar("umbral", sid, &umbral_borde, 13);
}

void nodo_canny::procesar()
{
    if (!m_src.empty())
    {
        /* por qué por 3? */
        cv::Canny(m_src, m_out, umbral_borde, umbral_borde * 3);
    }
};

/************************************************/

nodo_laplace::nodo_laplace(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(230, 230, 230);
    sid = "Laplace" + sid;
    cv::namedWindow(sid);
    cv::createTrackbar("sigma", sid, &sigma, 13);
    cv::createTrackbar("scale", sid, &scale, 5);
}

void nodo_laplace::procesar()
{
    if (!m_src.empty())
    {
        int ksize = sigma | 1;
        cv::GaussianBlur(m_src, smooth, cv::Size(ksize, ksize), sigma, sigma);
        cv::Laplacian(smooth, laplaciana, CV_16S, ksize, scale, 0);
        convertScaleAbs(laplaciana, m_out, (sigma + 1) * 0.25);
    }
};

/************************************************/

nodo_hsv::nodo_hsv(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(23, 80, 230);
    sid = "HSV" + sid;
}

void nodo_hsv::procesar()
{
    if (!m_src.empty())
    {
        cv::cvtColor(m_src, m_out, cv::COLOR_BGR2HSV);
    }
};

/****************************************************/

nodo_erosion_dilacion::nodo_erosion_dilacion(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(23, 0, 30);
    sid = "erosion_dilacion" + sid;
    cv::namedWindow(sid);
    cv::createTrackbar("kernel_erosion", sid, &kernel_erosion_sz, 19);
    cv::createTrackbar("kernel_dilacion", sid, &kernel_dilacion_sz, 19);
}

void nodo_erosion_dilacion::procesar()
{
    if (!m_src.empty())
    {
        kernel_erosion_sz = kernel_erosion_sz < 1 ? 1 : kernel_erosion_sz;
        kernel_dilacion_sz = kernel_dilacion_sz < 1 ? 1 : kernel_dilacion_sz;
        auto kernel_erosion = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kernel_erosion_sz, kernel_erosion_sz));
        auto kernel_dilacion = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kernel_dilacion_sz, kernel_dilacion_sz));
        cv::erode(m_src, m_out, kernel_erosion, cv::Point(-1, -1), 2);
        cv::dilate(m_out, m_out, kernel_dilacion, cv::Point(-1, -1), 2);
    }
}

/*******************************************************/
nodo_mascara::nodo_mascara(cv::Point c, int r)
    : nodo(c, r)
    , hl(0), sl(95), vl(96)
    , hh(20), sh(200), vh(175)
{
    mcolor = cv::Scalar(0, 255, 200);
    sid = "mascara" + sid;
    cv::namedWindow(sid);
    cv::createTrackbar("Hue L", sid, &hl, 255);
    cv::createTrackbar("Hue H", sid, &hh, 255);
    cv::createTrackbar("Sat L", sid, &sl, 255);
    cv::createTrackbar("Sat H", sid, &sh, 255);
    cv::createTrackbar("Val L", sid, &vl, 255);
    cv::createTrackbar("Val H", sid, &vh, 255);
}

void nodo_mascara::procesar()
{
    if (!m_src.empty())
    {
        cv::Scalar lower = cv::Scalar(hl, sl, vl);
        cv::Scalar upper = cv::Scalar(hh, sh, vh);
        cv::inRange(m_src, lower, upper, m_out);
    }
}

/********************************************************/

nodo_bitwise_and::nodo_bitwise_and(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(255, 0, 0);
    sid = "bitwise_and" + sid;
}

void nodo_bitwise_and::suscribir_a(nodo *src)
{
    if (src != nullptr)
    {
        std::cout << "suscribiendo " << sid << " a " << src->sid << ". " << src->sid << " -> " << sid << std::endl;
        proveedores.push_back(src);
        src->suscriptores.push_back(this);
        if (src->m_out.type() == 0)
            mmascara = src->m_out;
        else
            m_src = src->m_out;
    }
}

void nodo_bitwise_and::procesar()
{
    if (proveedores.size() >= 2)
    {
        m_out = cv::Scalar(0, 0, 0);
        cv::bitwise_and(m_src, m_src, m_out, mmascara);
        //cv::bitwise_and(proveedores[0]->m_out, proveedores[1]->m_out, m_out);
    }
}

/*******************************************************************************/

nodo_filtro_bilateral::nodo_filtro_bilateral(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(123, 123, 60);
    sid = "Filtro_bilateral" + sid;
    cv::namedWindow(sid);
    cv::createTrackbar("d", sid, &d, 10);
    cv::createTrackbar("Sigma Color", sid, &sigmaColor, 500);
    cv::createTrackbar("Sigma Space", sid, &sigmaSpace, 500);
}
void nodo_filtro_bilateral::procesar()
{
    if (!m_src.empty())
    {
        d = d < 1 ? 1 : d;
        cv::bilateralFilter(m_src, m_out, d, sigmaColor, sigmaSpace);
    }
}

/*******************************************************************************/

nodo_hough_circulo::nodo_hough_circulo(cv::Point c, int r) : nodo(c, r)
{
    mcolor = cv::Scalar(127, 200, 40);
    sid = "Hough circulo" + sid;
}

void nodo_hough_circulo::procesar()
{
    if (!m_src.empty())
    {
        m_out = m_src.clone();
        cv::HoughCircles(m_src, circulos, cv::HOUGH_GRADIENT, 2, m_src.rows / 4, 200, 100);
        for (size_t i = 0; i < circulos.size(); i++)
        {
            cv::Point center(cvRound(circulos[i][0]), cvRound(circulos[i][1]));
            int radius = cvRound(circulos[i][2]);
            cv::circle(m_out, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
        }
    }
}
