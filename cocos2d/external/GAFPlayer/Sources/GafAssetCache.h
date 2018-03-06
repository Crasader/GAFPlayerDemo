//
//  GafAssetCache.h
//  TUDO
//
//  Created by tcdcr on 2015/04/15.
//
//  GafAsset 对象缓存，提高 gaf 对象加载性能
//

#ifndef __Tudo__GafAssetCache__
#define __Tudo__GafAssetCache__

#include "cocos2d.h"
#include <queue>
#include <vector>

USING_NS_CC;

namespace gaf
{
    class GAFAsset;
    class GafAssetCache final : public Ref
    {
    public:
        ~GafAssetCache();
        static GafAssetCache* getInstance();

        struct AsyncStruct
        {
            AsyncStruct(const std::string& fn, std::function<void(GAFAsset*)> f) : filename(fn), callback(f) {}
            std::string filename;
            std::function<void(GAFAsset*)> callback;
        };
    
        /**/
        void waitForQuit();
        /**/
        GAFAsset* addGafAsset(const std::string &);
        /**/
        void addGafAssetAsync(const std::string &, const std::function<void(GAFAsset*)>&);
        /**/
        void removeGafAssetForKey(const std::string &, bool isShowRef = false);
        /**/
        GAFAsset* getGafAssetForKey(const std::string &) const;
        /* */
        void removeUnusedAssets();
        
        std::string getGafMemoryInfo();
    
    private:
        GafAssetCache();
        GafAssetCache (const GafAssetCache&);
        GafAssetCache& operator=(const GafAssetCache&);
    
        /* 加载 GafAsset 的线程函数 */
        void loadGafAssetThread();
        /* 将 gafAsset 对象加入容器  */
        void addGafAssetAsyncCallBack(float);
        
    protected:
        typedef struct _GafAssetInfo
        {
            AsyncStruct *asyncStruct;
            GAFAsset    *gafAsset;
        } GafAssetInfo;
        
        std::thread* _loadingThread = nullptr;
        std::mutex _sleepMutex;
        std::condition_variable _sleepCondition;
        bool _needQuit = false;
        int _asyncRefCount = 0;
        std::unordered_map<std::string, GAFAsset*> _gafAssets;

        std::queue<AsyncStruct*>* _asyncStructQueue = nullptr;
        std::mutex _asyncStructQueueMutex;
        std::deque<GafAssetInfo*>* _gafAssetInfoQueue = nullptr;
        std::mutex gafAssetInfoMutex;
    };
}

#endif /* defined(__Tudo__GafAssetCache__) */

