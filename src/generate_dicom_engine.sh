
trymkdir() {
    if [ ! -e $1 ];then
        mkdir -p $1
    fi
}

# 复制thirdparty相关文件到build文件夹
echo ">>>>> 正在复制libcharls..."
cp thirdparty/libcharls.so build/libcharls.so
echo ">>>>> 正在复制libImagingEngineLib..."
cp thirdparty/libImagingEngineLib.so build/libImagingEngineLib.so

# 执行java的设置
#source /etc/profile
# 进入Java文件所在路径， 得到.class .h 
clear
cd SvrJava
echo ">>>>> 正在 jni..."
javac ./com/deepwise/cta/component/rebuild/SvrCallImageEngine.java
javah -jni com.deepwise.cta.component.rebuild.SvrCallImageEngine
# 返回到批处理文件所在的路径
cd ..

cd ..

echo ">>>>> 正在生成libcontroller.so..."
# 进入build文件夹，开始生成.so
trymkdir build
cd build
cmake ..
make -j

# 返回到批处理文件所在的路径
cd ..

