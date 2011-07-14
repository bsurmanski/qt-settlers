/* 
 * File:   Sprite.cpp
 * Author: brandon
 * 
 * Created on May 7, 2011, 1:07 PM
 */

#include "Sprite.h"
#include "GameLibrary.h"

/**
 * currently not used
 */
Sprite::Sprite() {
    easing = false;
    corner1 = Vector2f(-0.4, -0.4);
    corner2 = Vector2f(0.4, 0.4);
    size = 0;
    state = 0;
    color = Vector3f(1, 1, 1);
}

Sprite::Sprite(std::string filename) {
    easing = false;
    corner1 = Vector2f(-0.4, -0.4);
    corner2 = Vector2f(0.4, 0.4);
    texture = new GLuint[1];

    file = filename;
    texture[0] = GameLibrary::getInstance()->getCachedTexture(filename);

    size = 1;
    state = 0;
    color = Vector3f(1, 1, 1);
}

Sprite::~Sprite() {
    delete texture;
}

void Sprite::setSize(Vector2f dimension) {
    corner1 = -dimension / 2;
    corner2 = dimension / 2;
}

void Sprite::setPosition(Vector3f position) {
    this->position = position;
}

void Sprite::addState(std::string filename) {
    GLuint* newTexArray = new GLuint[size + 1];
    for (int i = 0; i < size; i++) {
        newTexArray[i] = texture[i];
    }
    delete [] texture;
    newTexArray[size] = GameLibrary::getInstance()->getCachedTexture(filename);
    texture = newTexArray;
    size++;
}

void Sprite::changeState(int toState) {
    if (toState >= size)
        throw;
    state = toState;
}

void Sprite::setTint(Vector3f tint) {
    color = tint;
}

GLuint Sprite::loadTexture(std::string filename) {
    filename = "Resources/" + filename;
    GLuint tex;


    // glEnable(GL_TEXTURE_2D); // Enable texturing
    glGenTextures(1, &tex); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, tex); // Set as the current texture

    QImage im;
    assert(im.load(filename.c_str()));

    QImage texture = QGLWidget::convertToGLFormat(im);
    assert(!texture.isNull());

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //glDisable(GL_TEXTURE_2D);
    return tex;
}

/*GLuint Sprite::loadTexture(std::string fileName) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int w, h;
    GLubyte *pixels = (GLubyte *) load_png(fileName, &w, &h);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    free(pixels);

    return tex;
}

char* Sprite::load_png(std::string fileName, int *width, int *height) {
    fileName = "Resources/" + fileName;
    FILE *png_file = fopen(fileName.c_str(), "rb");
    //assert(png_file.open);

    uint8_t header[PNG_SIG_BYTES];

    fread(header, 1, PNG_SIG_BYTES, png_file);
    assert(!png_sig_cmp(header, 0, PNG_SIG_BYTES));

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    assert(png_ptr);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    assert(info_ptr);

    png_infop end_info = png_create_info_struct(png_ptr);
    assert(end_info);

    assert(!setjmp(png_jmpbuf(png_ptr)));
    png_init_io(png_ptr, png_file);
    png_set_sig_bytes(png_ptr, PNG_SIG_BYTES);
    png_read_info(png_ptr, info_ptr);

 *width = png_get_image_width(png_ptr, info_ptr);
 *height = png_get_image_height(png_ptr, info_ptr);

    png_uint_32 bit_depth, color_type;
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);

    //  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    //    png_set_gray_1_2_4_to_8(png_ptr);

    if (bit_depth == 16)
        png_set_strip_16(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    else if (color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
    }

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    else
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    png_read_update_info(png_ptr, info_ptr);

    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_uint_32 numbytes = rowbytes * (*height);
    png_byte* pixels = (png_byte*) malloc(numbytes);
    png_byte** row_ptrs = (png_byte**) malloc((*height) * sizeof (png_byte*));

    int i;
    for (i = 0; i < (*height); i++)
        row_ptrs[i] = pixels + ((*height) - 1 - i) * rowbytes;


    png_read_image(png_ptr, row_ptrs);

    free(row_ptrs);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(png_file);

    return (char *) pixels;
}
 */

void Sprite::easeTo(Vector3f location) {
    target = location;
    easing = true;
}

bool Sprite::Update() {
    if (easing) {
        position += ((target - position) / EASE_CONST);
        if ((target - position).length() <= EPSILON) {
            position = target;
            easing = false;
        }
        return true;
    }
    return false;
}

void Sprite::Draw() {
    if (texture[0] == -1) {
        texture[0] = GameLibrary::getInstance()->getCachedTexture(file);
    }

    glPushMatrix();
    glTranslatef(0, 0, 0.1);
    glColor3f(color.r, color.g, color.b);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[state]);

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(position.x + corner1.x, position.y + corner1.y, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(position.x + corner2.x, position.y + corner1.y, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(position.x + corner2.x, position.y + corner2.y, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(position.x + corner1.x, position.y + corner2.y, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

