#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

// Screen dimensions
#define WIDTH 80
#define HEIGHT 40
#define PI 3.14159265

// Cube vertices
float cubeVertices[8][3] = {
    {-1, -1, -1},  // 0
    { 1, -1, -1},  // 1
    { 1,  1, -1},  // 2
    {-1,  1, -1},  // 3
    {-1, -1,  1},  // 4
    { 1, -1,  1},  // 5
    { 1,  1,  1},  // 6
    {-1,  1,  1}   // 7
};

// Cube edges (connections between vertices)
int cubeEdges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},  // Bottom face
    {4, 5}, {5, 6}, {6, 7}, {7, 4},  // Top face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}   // Connecting edges
};

// Function to rotate point around X axis
void rotateX(float *x, float *y, float *z, float angle) {
    float tempY = *y * cos(angle) - *z * sin(angle);
    float tempZ = *y * sin(angle) + *z * cos(angle);
    *y = tempY;
    *z = tempZ;
}

// Function to rotate point around Y axis
void rotateY(float *x, float *y, float *z, float angle) {
    float tempX = *x * cos(angle) + *z * sin(angle);
    float tempZ = -(*x) * sin(angle) + *z * cos(angle);
    *x = tempX;
    *z = tempZ;
}

// Function to rotate point around Z axis
void rotateZ(float *x, float *y, float *z, float angle) {
    float tempX = *x * cos(angle) - *y * sin(angle);
    float tempY = *x * sin(angle) + *y * cos(angle);
    *x = tempX;
    *y = tempY;
}

// Function to project 3D point to 2D screen coordinates
void projectPoint(float x, float y, float z, int *screenX, int *screenY) {
    // Simple perspective projection
    float distance = 5.0;
    float factor = distance / (distance + z);
    
    // Project to 2D
    float projectedX = x * factor * 20;
    float projectedY = y * factor * 10;
    
    // Center on screen
    *screenX = (int)(projectedX + WIDTH / 2);
    *screenY = (int)(projectedY + HEIGHT / 2);
}

// Function to draw line in buffer
void drawLine(char buffer[HEIGHT][WIDTH], int x1, int y1, int x2, int y2) {
    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        // Check if point is within screen bounds
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            buffer[y1][x1] = '#';
        }
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Clear the screen buffer
void clearBuffer(char buffer[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            buffer[y][x] = ' ';
        }
    }
}

// Print the buffer to screen
void printBuffer(char buffer[HEIGHT][WIDTH]) {
    // Clear screen (for CXX Droid)
    printf("\033[2J\033[H");  // ANSI escape codes to clear screen
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(buffer[y][x]);
        }
        putchar('\n');
    }
}

int main() {
    char screenBuffer[HEIGHT][WIDTH];
    float angleX = 0, angleY = 0, angleZ = 0;
    float speedX = 0.03, speedY = 0.02, speedZ = 0.01;
    
    printf("3D Rotating Cube in C\n");
    printf("Press Ctrl+C to exit\n\n");
    sleep(2);
    
    while (1) {
        // Clear the buffer
        clearBuffer(screenBuffer);
        
        // Transform and project all vertices
        float transformedVertices[8][3];
        int screenPoints[8][2];
        
        for (int i = 0; i < 8; i++) {
            // Copy original vertex
            transformedVertices[i][0] = cubeVertices[i][0];
            transformedVertices[i][1] = cubeVertices[i][1];
            transformedVertices[i][2] = cubeVertices[i][2];
            
            // Apply rotations
            rotateX(&transformedVertices[i][0], &transformedVertices[i][1], &transformedVertices[i][2], angleX);
            rotateY(&transformedVertices[i][0], &transformedVertices[i][1], &transformedVertices[i][2], angleY);
            rotateZ(&transformedVertices[i][0], &transformedVertices[i][1], &transformedVertices[i][2], angleZ);
            
            // Project to 2D
            projectPoint(transformedVertices[i][0], 
                        transformedVertices[i][1], 
                        transformedVertices[i][2], 
                        &screenPoints[i][0], 
                        &screenPoints[i][1]);
        }
        
        // Draw all edges
        for (int i = 0; i < 12; i++) {
            int v1 = cubeEdges[i][0];
            int v2 = cubeEdges[i][1];
            
            drawLine(screenBuffer, 
                    screenPoints[v1][0], screenPoints[v1][1],
                    screenPoints[v2][0], screenPoints[v2][1]);
        }
        
        // Draw vertices as dots
        for (int i = 0; i < 8; i++) {
            int x = screenPoints[i][0];
            int y = screenPoints[i][1];
            
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                screenBuffer[y][x] = 'O';
            }
        }
        
        // Add info text
        char info[50];
        snprintf(info, sizeof(info), "Rotating Cube - X:%.2f Y:%.2f Z:%.2f", angleX, angleY, angleZ);
        for (int i = 0; info[i] != '\0' && i < WIDTH; i++) {
            if (i < HEIGHT) screenBuffer[0][i] = info[i];
        }
        
        // Display the buffer
        printBuffer(screenBuffer);
        
        // Update angles
        angleX += speedX;
        angleY += speedY;
        angleZ += speedZ;
        
        // Wrap angles to prevent overflow
        if (angleX > 2 * PI) angleX -= 2 * PI;
        if (angleY > 2 * PI) angleY -= 2 * PI;
        if (angleZ > 2 * PI) angleZ -= 2 * PI;
        
        // Small delay for animation (adjust for your device)
        usleep(30000);  // 30ms delay
    }
    
    return 0;
}