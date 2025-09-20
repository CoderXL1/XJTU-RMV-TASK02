# XJTU-RMV-TASK02
### OpenCV 配置+项目
--------
##### ~一帆风顺~ 肯定不可能啊
大概遇到了安装 OpenCV 后 `#include <opencv2/opencv.hpp>` 无法被 vscode intellisense 和编译器找到的问题。

后来通过问 AI 了解了如何在 CMakeLists 里面添加 opencv 的 flags，解决了无法编译的问题。同时也使用 `sudo ln -s /usr/include/opencv4/opencv2 /usr/include/opencv2` 来解决 vscode intellisense 报错的问题（尝试过修改 `c_cpp_properties.json` 但无效）

接下来以任务说明 pdf 为主，OpenCV 文档为辅，加上自己的思考和理解，较为顺利地写出了装甲板识别代码。

--------

#### 完成思路
现做预处理：高斯模糊 $\to$ 转为灰度图 $\to$ 二值化 $\to$ 先开再闭 $\to$ 通过 Canny 得到边缘 $\to$ 得到 contours $\to$ 对于每个 contours 进行 minAreaRect $\to$ 通过面积、长宽比、角度筛选出来装甲板