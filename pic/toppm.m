filename = '50608556_p0';
pic = imread([filename  '.png']);
pic = imresize(pic, [512 512]);
imwrite(pic, [filename '.ppm'], 'ppm');