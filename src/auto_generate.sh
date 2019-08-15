trymkdir() {
    if [ ! -e $1 ];then
        mkdir -p $1
    fi
}

echo ">>>>> 正在 生成ImagingEngineLib.so..."
cd ImagingEngineLib
trymkdir build
cd build
cmake ..
make -j

cd ../..
trymkdir build
./generate_dicom_engine.sh

echo ">>>>> 正在 复制build/.so 到output文件夹..."
trymkdir output
cp ./build/libcharls.so ./output/
cp ./build/libcontroller.so ./output/
cp ./build/libdicom_engine.so ./output/
cp ./build/libImagingEngineLib.so ./output/
cp ./build/libSvrCallImageEngine.so ./output/
