#include "LUtil.h"
#include "string"
#include "sstream"
#include "math.h"
#include "iostream"

int score_left = 0;
int score_right = 0;
// rackets
float racket_width = 0.1f;
float racket_height = 0.4f;
float racket_speed = 0.1f;

// left racket
float racket_left_x = -1.0f;
float racket_left_y = -0.2f;

// right racket
float racket_right_x = 1.0f - racket_width;
float racket_right_y = -0.2f;

// ball
float ball_pos_x = 0.0f;
float ball_pos_y = 0.0f;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
float ball_size = 0.08f;
float ball_speed = 0.00003f;

std::string int2str(int x) {
    // converts int into string
    std::stringstream ss;
    ss << x;
    return ss.str( );
}

void drawText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

void vec2_norm(float& x, float &y) {
    float length = sqrt((x * x) + (y * y));
    if (length != 0.0f) {
        length = 0.5f / length;
        x *= length;
        y *= length;
        ball_dir_x = x;
        ball_dir_y = y;
        }
    }

void updateBall()
{
    // move the ball
    ball_pos_x += ball_dir_x * ball_speed;
    ball_pos_y += ball_dir_y * ball_speed;
    // left racket
    if (ball_pos_x < racket_left_x + racket_width &&
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y) {
        // change direction depending on where it hit the racket
        float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
        ball_dir_x = 1.0f; // force it to be positive
        ball_dir_y = t;
    }
    // right racket
    if (ball_pos_x > racket_right_x-ball_size &&
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y) {
        // change direction depending on where it hit the racket
        float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
        ball_dir_x = -1.0f; // force it to be negative
        ball_dir_y = t;
    }
    // left wall
    if (ball_pos_x < -1.0f) {
        ++score_right;
        ball_pos_x = 0.0f;
        ball_pos_y = 0.0f;
        racket_left_x = -1.0f;
        racket_left_y = -0.2f;
        racket_right_x = 1.0f - racket_width;
        racket_right_y = -0.2f;
        ball_dir_x = -1.0f;
        ball_dir_y = 0;
        Sleep(1000);
    }
    // right wall
    if (ball_pos_x > 1.0f) {
        ++score_left;
        ball_pos_x = 0.0f;
        ball_pos_y = 0.0f;
        racket_left_x = -1.0f;
        racket_left_y = -0.2f;
        racket_right_x = 1.0f - racket_width;
        racket_right_y = -0.2f;
        ball_dir_x = 1.0f;
        ball_dir_y = 0;
        Sleep(1000);

    }
    // top wall
    if (ball_pos_y > 1.0f-ball_size) {
        ball_dir_y = -fabs(ball_dir_y);
    }
    // bottom wall
    if (ball_pos_y < -1.0f) {
        ball_dir_y = fabs(ball_dir_y);
    }

    vec2_norm(ball_dir_x, ball_dir_y);

}

void update(int value)
{
    updateBall();

    glutTimerFunc(1000/60, update, 0);

    // Redisplay frame
    glutPostRedisplay();
}

void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    // draw rackets
    drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
    drawRect(racket_right_x, racket_right_y, racket_width, racket_height);
    //draw ball
    drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);

     // draw score
    drawText(-0.0f, 0.9f, int2str(score_left) + ":" + int2str(score_right));

    //Update screen
    glutSwapBuffers();
    if (score_left >= 10 or score_right >= 10) {
        std::cout<<score_left<<":"<<score_right<<"is the final score";
        Sleep(2000);
        exit(1);
    }
}

void handleKeys( unsigned char key, int x, int y )
{
    //If the user presses keys
    if( key == 'w' )racket_left_y += racket_speed;
    else if (key=='s') racket_left_y -= racket_speed;
    else if (key=='k')racket_right_y += racket_speed;
    else if (key=='m') racket_right_y -= racket_speed;
    else if (key== 'q') exit(1);
    //Update projection matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
}
