%{
    Stereo vision image processing program for use with
    robotic arm for
    ECE 5397: Introduction to Robotics
    University of Houston, Fall 2017
    Authors: Nick Anderson & Sikender Shahid
    Course Instructor: Dr. Aaron Becker

    NOTE: This program was intended to calculate the coordinates of an
            object based on the input from two cameras and to send directions
            to an Arduino controlling a robotic arm to pick up the object.
            This program was not completed due to time constraints and
            other setbacks.
          
%}

clear all
clc

% Initialize and configure webcams
camL = webcam(2);
set(camL, 'Brightness', 60, 'Sharpness', 1)

camR = webcam(3);
set(camR, 'Brightness', 60, 'Sharpness', 1)

% Configure and open connection to Arduino
bot = serial('COM8', 'BaudRate', 9600, 'Terminator', 'CR');
fopen(bot)

% Execute FindObject function when the terminator character is read 
% from the serial port
bot.BytesAvailableFcnMode = 'terminator';
bot.BytesAvailableFcn = @FindObject;

% Image threshoolding limits
lThresh = 100;
rThresh = 100;

% Coordinate calculation constants
b = 95;                 % Horizontal distance between camera centers

lambda = 5.0;           % Camera focal length (assumes identical cameras)
                        % We were unable to accurately calculate this value
                        
mmPpix = 3.0 / 640;     % Conversion from pixels to milimeters
                        % We were unable to accurately calculate this value
                        
xCent = 640 / 2;        % Horizontal image center

% Send "Home Position" command to Arduino.
% NOTE: This was meant to act as a trigger to exit the Arduino
%   "setup" function and start the "loop" function, but was not tested and
%   may not work as intended.
fprintf(bot, 'HM');

% FindObject function for calculating the coordinates of an object and
% sending the proper command to the Arduino
% NOTE: This code uses color thresholding and is written for a red object
function FindObject(bot, event)
    % Initialize variables
    rowCentL = 0; % Row coordinate of the object centroid in the left camera
    colCentL = 0; % Column coordinate of the object centroid in the left camera
    cL = 0;       % Number of pixels in the object as seen by the left camera
    
    rowCentR = 0; % Row coordinate of the object centroid in the right camera
    colCentR = 0; % Column coordinate of the object centroid in the right camera
    cR = 0;       % Number of pixels in the object as seen by the right camera
    
    % Take a snapshot from each camera
    imgL = snapshot(camL);
    imgR = snapshot(camR);
    
    % Separate the left image into RGB layers threshold the image
    rL = imgL(:, :, 1);
    gL = imgL(:, :, 2);
    bL = imgL(:, :, 3);
    threshL = rL - ((gL + bL) / 2);
    
    % Separate the lright image into RGB layers threshold the image
    rR = imgR(:, :, 1);
    gR = imgR(:, :, 2);
    bR = imgR(:, :, 3);
    threshR = rR - ((gR + bR) / 2);
    
    % Calculate the centroid of the object in each image
    for i = 1:480
        for j = 1:640
            if threshL(i, j) >= lThresh
                rowCentL = rowCentL + i;
                colCentL = colCentL + j;
                cL = cL + 1;
            end
            
            if threshR(i, j) >= rThresh
                rowCentR = rowCentR + i;
                colCentR = colCentR + j;
                cR = cR + 1;
            end
        end
    end
    
    rowCentL = rowCentL / cL;
    colCentL = colCentL / cL;    
    
    rowCentR = rowCentR / cR;
    colCentR = colCentR / cR;
    
    % Insert centroid markers into each image 
    if cL > 0
        imgL = insertMarker(imgL,[colCentL rowCentL],'o','color','black','size',5);
    end
    if cR > 0
        imgR = insertMarker(imgR,[colCentR rowCentR],'o','color','black','size',5);
    end
   
    % Display the original images with centroid markers and the thresholded
    % images
    subplot(2,2,1)
    imshow(imgL)
    hold on
            
    subplot(2,2,2)
    imshow(imgR)
    hold on
    
    subplot(2,2,3)
    imshow(threshL > lThresh)
    hold on
    
    subplot(2,2,4)
    imshow(threshR > rThresh)
    hold on
    
    % Calculate the distance (in mm) of the object from the cameras
    uCentL = (colCentL - xCent) * mmPpix;
    uCentR = (colCentR - xCent) * mmPpix;
    z = b * lambda / (uCentL - uCentR);
    
    % Send appropriate instruction to Arduino:
    % Turn right if the object is visible in the right camera but not the
    %   left
    % Turn left if the object is visible in the left camera but not the
    %   right
    % Pull robot forward if object is visible in both cameras and is more
    %   than 170mm away.
    % If object is visible in both cameras and is within 170mm away, send
    % "pick up" command followed by object depth and horizontal coordinates.
    if colCentL == 0 && colCentR ~= 0
        fprintf(bot, 'RT');
    
    elseif colCentL ~= 0 && colCentR == 0
        fprintf(bot, 'LT');
        
    else
        if z > 170
            fprintf(bot, 'PF');
            
        else
            fprintf(bot, 'PU');
            fprintf(bot, '%f %f %f', z, uCentL, uCentR);
        end
    end
end