#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PI 3.14159265358979323846
#define WIDTH 80
#define HEIGHT 40
#define DEPTH 100

// 3D point structure
typedef struct {
    float x, y, z;
} Point3D;

// Cube vertices (8 corners)
Point3D cube[] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
    {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
};

// Cube edges (12 edges connecting vertices)
int edges[][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},  // Bottom face
    {4,5}, {5,6}, {6,7}, {7,4},  // Top face
    {0,4}, {1,5}, {2,6}, {3,7}   // Connecting edges
};

// Function to rotate point around X axis
void rotateX(Point3D *p, float angle) {
    float y = p->y * cos(angle) - p->z * sin(angle);
    float z = p->y * sin(angle) + p->z * cos(angle);
    p->y = y;
    p->z = z;
}

// Function to rotate point around Y axis
void rotateY(Point3D *p, float angle) {
    float x = p->x * cos(angle) + p->z * sin(angle);
    float z = -p->x * sin(angle) + p->z * cos(angle);
    p->x = x;
    p->z = z;
}

// Function to rotate point around Z axis
void rotateZ(Point3D *p, float angle) {
    float x = p->x * cos(angle) - p->y * sin(angle);
    float y = p->x * sin(angle) + p->y * cos(angle);
    p->x = x;
    p->y = y;
}

// Project 3D point to 2D screen coordinates
void project(Point3D p, int *screenX, int *screenY, int *screenZ) {
    // Perspective projection
    float factor = DEPTH / (DEPTH + p.z * 5);
    *screenX = (int)(p.x * factor * (WIDTH/4) + WIDTH/2);
    *screenY = (int)(p.y * factor * (HEIGHT/4) + HEIGHT/2);
    *screenZ = (int)((p.z + 2) * 10);  // For depth sorting (not used in this simple version)
}

// Clear screen function
void clearScreen() {
    printf("\033[2J\033[H");  // ANSI escape codes to clear screen
}

// Draw the cube
void drawCube(float angleX, float angleY, float angleZ) {
    char screen[HEIGHT][WIDTH];
    int i, j;
    
    // Initialize screen with spaces
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            screen[i][j] = ' ';
        }
    }
    
    // Create rotated copy of cube vertices
    Point3D rotatedCube[8];
    for (i = 0; i < 8; i++) {
        rotatedCube[i] = cube[i];
        rotateX(&rotatedCube[i], angleX);
        rotateY(&rotatedCube[i], angleY);
        rotateZ(&rotatedCube[i], angleZ);
    }
    
    // Draw edges
    for (i = 0; i < 12; i++) {
        int start = edges[i][0];
        int end = edges[i][1];
        
        int x1, y1, z1, x2, y2, z2;
        project(rotatedCube[start], &x1, &y1, &z1);
        project(rotatedCube[end], &x2, &y2, &z2);
        
        // Draw line using Bresenham's algorithm (simplified)
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        while (1) {
            if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
                // Use different characters for different edges
                char edgeChar = '#';
                if (i < 4) edgeChar = '-';  // Bottom face
                else if (i < 8) edgeChar = '=';  // Top face
                else edgeChar = '|';  // Vertical edges
                
                screen[y1][x1] = edgeChar;
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
    
    // Draw vertices
    for (i = 0; i < 8; i++) {
        int x, y, z;
        project(rotatedCube[i], &x, &y, &z);
        
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            screen[y][x] = 'O';
        }
    }
    
    // Print the screen
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            putchar(screen[i][j]);
        }
        putchar('\n');
    }
}

// Simple version with rotation on all axes
void simpleCubeRotation() {
    float angleX = 0, angleY = 0, angleZ = 0;
    
    printf("\033[2J");  // Clear screen
    
    while (1) {
        clearScreen();
        
        // Display title
        printf("\n    3D CUBE ROTATION DEMO\n");
        printf("    Press Ctrl+C to exit\n\n");
        
        drawCube(angleX, angleY, angleZ);
        
        // Display angles
        printf("\n    Rotation Angles: X=%.2f  Y=%.2f  Z=%.2f", 
               angleX * 180/PI, angleY * 180/PI, angleZ * 180/PI);
        
        // Update angles
        angleX += 0.05;
        angleY += 0.03;
        angleZ += 0.02;
        
        // Keep angles in range
        if (angleX > 2*PI) angleX -= 2*PI;
        if (angleY > 2*PI) angleY -= 2*PI;
        if (angleZ > 2*PI) angleZ -= 2*PI;
        
        usleep(50000);  // 50ms delay
    }
}

// Alternative: User-controlled rotation
void interactiveCubeRotation() {
    float angleX = 0, angleY = 0, angleZ = 0;
    int frame = 0;
    
    printf("\033[2J");  // Clear screen
    
    while (1) {
        clearScreen();
        
        printf("\n    3D CUBE ROTATION (Frame: %d)\n", ++frame);
        printf("    Auto-rotating... Press Ctrl+C to exit\n\n");
        
        drawCube(angleX, angleY, angleZ);
        
        // Display rotation info
        printf("\n    Rotation around all three axes");
        printf("\n    X: sin wave | Y: linear | Z: cos wave");
        
        // Animate with different patterns
        angleX = sin(frame * 0.1) * PI;
        angleY = frame * 0.05;
        angleZ = cos(frame * 0.07) * PI;
        
        usleep(80000);  // 80ms delay
    }
}

int main() {
    // You can choose which animation to run
    int choice;
    
    printf("3D Cube Rotation Demo\n");
    printf("=====================\n");
    printf("1. Simple rotation (all axes)\n");
    printf("2. Interactive pattern rotation\n");
    printf("Enter choice (1 or 2): ");
    
    scanf("%d", &choice);
    
    if (choice == 1) {
        simpleCubeRotation();
    } else {
        interactiveCubeRotation();
    }
    
    return 0;
}