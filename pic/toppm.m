filename = '50650833_p0';
pic = imread([filename  '.jpg']);
pic = imresize(pic, 1);
imwrite(pic, [filename '.ppm'], 'ppm');