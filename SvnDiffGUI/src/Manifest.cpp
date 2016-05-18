/****************************************************************************
 Copyright (c) 2014 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "Manifest.h"
#include "json/filestream.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include "json/document.h"

#include <fstream>

#define KEY_VERSION             "version"
#define KEY_PACKAGE_URL         "packageUrl"
#define KEY_MANIFEST_URL        "remoteManifestUrl"
#define KEY_VERSION_URL         "remoteVersionUrl"
#define KEY_GROUP_VERSIONS      "groupVersions"
#define KEY_ENGINE_VERSION      "engineVersion"
#define KEY_ASSETS              "assets"
#define KEY_COMPRESSED_FILES    "compressedFiles"
#define KEY_SEARCH_PATHS        "searchPaths"

#define KEY_PATH                "path"
#define KEY_MD5                 "md5"
#define KEY_GROUP               "group"
#define KEY_COMPRESSED          "compressed"
#define KEY_COMPRESSED_FILE     "compressedFile"
#define KEY_DOWNLOAD_STATE      "downloadState"

#define KEY_VER_NAME   "vername"
#define  KEY_VER_SIZE   "versize"
#define  KEY_FIX_VERSION "fixversion"

#include "Tool.h"

Manifest::Manifest(const std::string& manifestUrl/* = ""*/)
: _versionLoaded(false)
, _loaded(false)
, _manifestRoot("")
, _remoteManifestUrl("")
, _remoteVersionUrl("")
, _version("")
, _engineVer("")
{

}

void Manifest::loadJson(const std::string& content)
{
		clear();
        if (content.size() == 0)
        {
        }
        else
        {
            // Parse file with rapid json
            _json.Parse<0>(content.c_str());
            // Print error
            if (_json.HasParseError()) {
                size_t offset = _json.GetErrorOffset();
                if (offset > 0)
                    offset--;
                std::string errorSnippet = content.substr(offset, 10);
            }
        }
}

void Manifest::parseVersion(const std::string& content)
{
	loadJson(content);
    
    if (_json.IsObject())
    {
        loadVersion(_json);
    }
}

void Manifest::parse(const std::string& content)
{
	loadJson(content);
	
    if (_json.IsObject())
    {
        loadManifest(_json);
    }
}

bool Manifest::isVersionLoaded() const
{
	return _versionLoaded;
}
bool Manifest::isLoaded() const
{
	return _loaded;
}

bool Manifest::versionEquals(const Manifest *b) const
{
    // Check manifest version
    if (_version != b->getVersion())
    {
        return false;
    }
    // Check group versions
    else
    {
        std::unordered_map<std::string, Manifest::GroupVersionValue> bGroupVer = b->getGroupVerions();

		
		if (bGroupVer.size() != _groupVer.size())
		{
			return false;
		}
		std::unordered_map<std::string, Manifest::GroupVersionValue>::const_iterator  groupIt;
		for (groupIt = bGroupVer.begin(); groupIt != bGroupVer.end();groupIt ++)
		{
			std::string  key = groupIt->first;
			GroupVersionValue  bGroupVersion = groupIt->second;
			if (_groupVer.find(key)==_groupVer.cend())
			{
				return false;
			}
			GroupVersionValue  aGroupVersion = _groupVer.find(key)->second;
			if (strcmp(aGroupVersion.vername.c_str(), bGroupVersion.vername.c_str())!=0 || aGroupVersion.versize!=bGroupVersion.versize)
			{
				return false;
			}
		}
		for (groupIt = _groupVer.begin(); groupIt != _groupVer.end();groupIt++)
		{
			std::string  key = groupIt->first;
			if (bGroupVer.find(key) == bGroupVer.cend())
			{
				return false;
			}

		}
    }
    return true;
}

std::unordered_map<std::string, Manifest::AssetDiff> Manifest::genDiff(const Manifest *b) const
{
    std::unordered_map<std::string, AssetDiff> diff_map;
    std::unordered_map<std::string, Asset> bAssets = b->getAssets();
    
    std::string key;
    Asset valueA;
    Asset valueB;
    std::unordered_map<std::string, Asset>::const_iterator valueIt, it;
    for (it = _assets.begin(); it != _assets.end(); ++it)
    {
        key = it->first;
        valueA = it->second;
        
        // Deleted
        valueIt = bAssets.find(key);
        if (valueIt == bAssets.cend()) {
            AssetDiff diff;
            diff.asset = valueA;
            diff.type = DiffType::DELETED;
            diff_map.emplace(key, diff);
            continue;
        }
        
        // Modified
        valueB = valueIt->second;
        if (valueA.md5 != valueB.md5) {
            AssetDiff diff;
            diff.asset = valueB;
            diff.type = DiffType::MODIFIED;
            diff_map.emplace(key, diff);
        }
    }
    
    for (it = bAssets.begin(); it != bAssets.end(); ++it)
    {
        key = it->first;
        valueB = it->second;
        
        // Added
        valueIt = _assets.find(key);
        if (valueIt == _assets.cend()) {
            AssetDiff diff;
            diff.asset = valueB;
            diff.type = DiffType::ADDED;
            diff_map.emplace(key, diff);
        }
    }
    
    return diff_map;
}


std::vector<std::string> Manifest::getSearchPaths() const
{
    std::vector<std::string> searchPaths;
    searchPaths.push_back(_manifestRoot);
    
    for (int i = (int)_searchPaths.size()-1; i >= 0; i--)
    {
        std::string path = _searchPaths[i];
        if (path.size() > 0 && path[path.size() - 1] != '/')
            path.append("/");
        path = _manifestRoot + path;
        searchPaths.push_back(path);
    }
    return searchPaths;
}



const std::string& Manifest::getPackageUrl() const
{
    return _packageUrl;
}

const std::string& Manifest::getManifestFileUrl() const
{
    return _remoteManifestUrl;
}

const std::string& Manifest::getVersionFileUrl() const
{
    return _remoteVersionUrl;
}

const std::string& Manifest::getVersion() const
{
    return _version;
}
/*
const std::vector<std::string>& Manifest::getGroups() const
{
    return _groups;
}
*/
const std::unordered_map<std::string, Manifest::GroupVersionValue>& Manifest::getGroupVerions() const
{
    return _groupVer;
}

const Manifest::GroupVersionValue& Manifest::getGroupVersion(const std::string &group) const
{
    return _groupVer.at(group);
}

const std::unordered_map<std::string, Manifest::Asset>& Manifest::getAssets() const
{
    return _assets;
}


void Manifest::clear()
{
    if (_versionLoaded || _loaded)
    {
        _groups.clear();
        _groupVer.clear();
        
        _remoteManifestUrl = "";
        _remoteVersionUrl = "";
        _version = "";
        _engineVer = "";
        
        _versionLoaded = false;
    }
    
    if (_loaded)
    {
        _assets.clear();
        _searchPaths.clear();
        _loaded = false;
    }
}

Manifest::Asset Manifest::parseAsset(const std::string &path, const rapidjson::Value &json)
{
    Asset asset;
    asset.path = path;
	
    if ( json.HasMember(KEY_MD5) && json[KEY_MD5].IsString() )
    {
        asset.md5 = json[KEY_MD5].GetString();
    }
    else asset.md5 = "";
    
    if ( json.HasMember(KEY_PATH) && json[KEY_PATH].IsString() )
    {
        asset.path = json[KEY_PATH].GetString();
    }
    
    if ( json.HasMember(KEY_COMPRESSED) && json[KEY_COMPRESSED].IsBool() )
    {
        asset.compressed = json[KEY_COMPRESSED].GetBool();
    }
    else asset.compressed = false;
    
    if ( json.HasMember(KEY_DOWNLOAD_STATE) && json[KEY_DOWNLOAD_STATE].IsInt() )
    {
        asset.downloadState = (DownloadState)(json[KEY_DOWNLOAD_STATE].GetInt());
    }
    else asset.downloadState = DownloadState::UNSTARTED;
    
    return asset;
}

Manifest::GroupVersionValue Manifest::parseGroupVersion(const rapidjson::Value &json)
{
	GroupVersionValue groupVersion;
	if ( json.HasMember(KEY_VER_NAME) && json[KEY_VER_NAME].IsString() )
	{
		groupVersion.vername = json[KEY_VER_NAME].GetString();
	}
	else{
		groupVersion.vername = "0";
	}

	if ( json.HasMember(KEY_VER_SIZE) && json[KEY_VER_SIZE].IsInt() )
	{
		groupVersion.versize = json[KEY_VER_SIZE].GetInt();
	}
	else{
		groupVersion.versize = 0;
	}
	return groupVersion;
}

void Manifest::loadVersion(const rapidjson::Document &json)
{
    // Retrieve remote manifest url
    if ( json.HasMember(KEY_MANIFEST_URL) && json[KEY_MANIFEST_URL].IsString() )
    {
        _remoteManifestUrl = json[KEY_MANIFEST_URL].GetString();
    }
    
    // Retrieve remote version url
    if ( json.HasMember(KEY_VERSION_URL) && json[KEY_VERSION_URL].IsString() )
    {
        _remoteVersionUrl = json[KEY_VERSION_URL].GetString();
    }
    
    // Retrieve local version
    if ( json.HasMember(KEY_VERSION) && json[KEY_VERSION].IsString() )
    {
        _version = json[KEY_VERSION].GetString();
    }
    
    // Retrieve local group version
    if ( json.HasMember(KEY_GROUP_VERSIONS) )
    {
        const rapidjson::Value& groupVers = json[KEY_GROUP_VERSIONS];
        if (groupVers.IsObject())
        {
            for (rapidjson::Value::ConstMemberIterator itr = groupVers.MemberonBegin(); itr != groupVers.MemberonEnd(); ++itr)
            {
                std::string group = itr->name.GetString();
				GroupVersionValue groupVersion = parseGroupVersion(itr->value);
				_groupVer.emplace(group, groupVersion);
            }
        }
    }
    
    // Retrieve local engine version
    if ( json.HasMember(KEY_ENGINE_VERSION) && json[KEY_ENGINE_VERSION].IsString() )
    {
        _engineVer = json[KEY_ENGINE_VERSION].GetString();
    }

	if (json.HasMember(KEY_FIX_VERSION) && json[KEY_FIX_VERSION].IsString())
	{
		_fixversion = json[KEY_FIX_VERSION].GetString();
	}
    
    _versionLoaded = true;
}

void Manifest::loadManifest(const rapidjson::Document &json)
{
    loadVersion(json);
    
    // Retrieve package url
    if ( json.HasMember(KEY_PACKAGE_URL) && json[KEY_PACKAGE_URL].IsString() )
    {
        _packageUrl = json[KEY_PACKAGE_URL].GetString();
        // Append automatically "/"
        if (_packageUrl.size() > 0 && _packageUrl[_packageUrl.size() - 1] != '/')
        {
            _packageUrl.append("/");
        }
    }
    
    // Retrieve all assets
    if ( json.HasMember(KEY_ASSETS) )
    {
        const rapidjson::Value& assets = json[KEY_ASSETS];
        if (assets.IsObject())
        {
            for (rapidjson::Value::ConstMemberIterator itr = assets.MemberonBegin(); itr != assets.MemberonEnd(); ++itr)
            {
                std::string key = itr->name.GetString();
                Asset asset = parseAsset(key, itr->value);
                _assets.emplace(key, asset);
            }
        }
    }
    
    // Retrieve all search paths
    if ( json.HasMember(KEY_SEARCH_PATHS) )
    {
        const rapidjson::Value& paths = json[KEY_SEARCH_PATHS];
        if (paths.IsArray())
        {
            for (rapidjson::SizeType i = 0; i < paths.Size(); ++i)
            {
                if (paths[i].IsString()) {
                    _searchPaths.push_back(paths[i].GetString());
                }
            }
        }
    }
    
    _loaded = true;
}

bool  Manifest::addAsset(std::string name, std::string path, std::string md5, bool isCompress)
{
	Asset assert;
	assert.path = path;
	assert.md5 = md5;
	assert.compressed = isCompress;
	if (name.length()<=0)
	{
		name = path;
	}
	
	
	if (_json.IsObject())
	{
		rapidjson::Document::AllocatorType& allocator = _json.GetAllocator();
		if (_json.HasMember(KEY_ASSETS))
		{
			rapidjson::Value& jsonAsset = _json[KEY_ASSETS];
			for (rapidjson::Value::ConstMemberIterator itr = jsonAsset.MemberonBegin(); itr != jsonAsset.MemberonEnd(); ++itr)
			{
				std::string key = itr->name.GetString();
				Asset asset = parseAsset(key, itr->value);
				if ((strcmp(key.c_str(), name.c_str()) != 0) || (strcmp(asset.path.c_str(), path.c_str()) != 0))
				{
					return false;
				}
			}

			rapidjson::Value jsonkey(rapidjson::kStringType);
			jsonkey.SetString(name.c_str(), allocator);

			rapidjson::Value  newitem(rapidjson::kObjectType);
			rapidjson::Value itempath(rapidjson::kStringType);
			itempath.SetString(path.c_str(),allocator);

			rapidjson::Value itemmd5(rapidjson::kStringType);
			itemmd5.SetString(md5.c_str(),allocator);

			newitem.AddMember(KEY_PATH, itempath,allocator);
			newitem.AddMember(KEY_MD5, itemmd5, allocator);
			newitem.AddMember(KEY_COMPRESSED, isCompress, allocator);

			jsonAsset.AddMember(jsonkey, newitem, allocator);

		}
		else{
			rapidjson::Value jsonAsset(rapidjson::kObjectType);

			rapidjson::Value jsonkey(rapidjson::kStringType);
			jsonkey.SetString(name.c_str(),allocator);

			rapidjson::Value  newitem(rapidjson::kObjectType);
			rapidjson::Value itempath(rapidjson::kStringType);
			itempath.SetString(path.c_str(),allocator);

			rapidjson::Value itemmd5(rapidjson::kStringType);
			itemmd5.SetString(md5.c_str(),allocator);

			newitem.AddMember(KEY_PATH, itempath, allocator);
			newitem.AddMember(KEY_MD5, itemmd5, allocator);
			newitem.AddMember(KEY_COMPRESSED, isCompress, allocator);

			jsonAsset.AddMember(jsonkey, newitem, allocator);

			_json.AddMember(KEY_ASSETS, jsonAsset, allocator);
		}
	}
	else{
		return false;
	}
	_assets.emplace(name, assert);
	return true;
}

bool Manifest::addGroupVersion(std::string name, std::string vername, size_t versize)
{
	GroupVersionValue groupVersion;
	groupVersion.vername = vername;
	groupVersion.versize = versize;
	
	rapidjson::Document::AllocatorType& allocator = _json.GetAllocator();
	if (_json.IsObject())
	{
		if (_json.HasMember(KEY_GROUP_VERSIONS))
		{
			rapidjson::Value&  jsonGroupVersion = _json[KEY_GROUP_VERSIONS];

			rapidjson::Value group(rapidjson::kObjectType);
			rapidjson::Value groupName(rapidjson::kStringType);
			groupName.SetString(vername.c_str(),allocator);
			rapidjson::Value groupSize(rapidjson::kNumberType);
			groupSize.SetInt(versize);
			group.AddMember(KEY_VER_NAME, groupName, allocator);
			group.AddMember(KEY_VER_SIZE, groupSize, allocator);

			rapidjson::Value jsonGroupKey(rapidjson::kStringType);
			jsonGroupKey.SetString(name.c_str(), allocator);
			jsonGroupVersion.AddMember(jsonGroupKey, group, allocator);
		}
		else{
			rapidjson::Value  jsonGroupVersion(rapidjson::kObjectType);

			rapidjson::Value group(rapidjson::kObjectType);
			rapidjson::Value groupName(rapidjson::kStringType);
			groupName.SetString(vername.c_str(),allocator);
			rapidjson::Value groupSize(rapidjson::kNumberType);
			groupSize.SetInt(versize);
			group.AddMember(KEY_VER_NAME, groupName, allocator);
			group.AddMember(KEY_VER_SIZE, groupSize, allocator);

			rapidjson::Value jsonGroupKey(rapidjson::kStringType);
			jsonGroupKey.SetString(name.c_str(), allocator);

			jsonGroupVersion.AddMember(jsonGroupKey, group, allocator);
			
			_json.AddMember(KEY_GROUP_VERSIONS, jsonGroupVersion, allocator);
		}
	}
	else{
		return false;
	}

	_groupVer.emplace(name, groupVersion);
	return true;
}

bool Manifest::removeAssets()
{
	if (_json.IsObject())
	{
		if (_json.HasMember(KEY_ASSETS))
		{
			_json.RemoveMember(KEY_ASSETS);
		}
		return true;
	}
	return false;
}


void Manifest::saveToFile(const std::string &filepath)
{
	tryMakeDir(filepath);
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    _json.Accept(writer);
    
    std::ofstream output(filepath, std::ofstream::out);
    if(!output.bad())
        output << buffer.GetString() << std::endl;
}


void Manifest::setVersion(std::string version)
{
	if (_json.HasMember(KEY_VERSION))
	{
		rapidjson::Value& jsonVersion = _json[KEY_VERSION];
		if (jsonVersion.IsString())
		{
			jsonVersion.SetString(version.c_str(),_json.GetAllocator());
		}
	}
	else{
			rapidjson::Value jsonVersion(rapidjson::kStringType);
			_json.AddMember(KEY_VERSION, jsonVersion,_json.GetAllocator());
	}
	_version = version;
}

void Manifest::setFixVersion(std::string fixversion)
{
	if (_json.HasMember(KEY_FIX_VERSION) && _json[KEY_FIX_VERSION].IsString())
	{
		_json[KEY_FIX_VERSION].SetString(fixversion.c_str(), _json.GetAllocator());
	}
	else{
		rapidjson::Value jsonFixVersion(rapidjson::kStringType);
		jsonFixVersion.SetString(fixversion.c_str(), _json.GetAllocator());
		_json.AddMember(KEY_FIX_VERSION, jsonFixVersion, _json.GetAllocator());
	}
}


void Manifest::setPackageUrl(std::string packageUrl)
{
	if (_json.HasMember(KEY_PACKAGE_URL) && _json[KEY_PACKAGE_URL].IsString())
	{
		_json[KEY_PACKAGE_URL].SetString(packageUrl.c_str(), _json.GetAllocator());
	}
	else{
		rapidjson::Value jsonPackageUrl(rapidjson::kStringType);
		jsonPackageUrl.SetString(packageUrl.c_str(),_json.GetAllocator());
		_json.AddMember(KEY_PACKAGE_URL, jsonPackageUrl, _json.GetAllocator());
	}
}

void Manifest::setManifestFileUrl(std::string manifestFileUrl)
{
	if (_json.HasMember(KEY_MANIFEST_URL) && _json[KEY_MANIFEST_URL].IsString())
	{
		_json[KEY_MANIFEST_URL].SetString(manifestFileUrl.c_str(), _json.GetAllocator());
	}
	else{
		rapidjson::Value jsonPackageUrl(rapidjson::kStringType);
		jsonPackageUrl.SetString(manifestFileUrl.c_str(), _json.GetAllocator());
		_json.AddMember(KEY_MANIFEST_URL, jsonPackageUrl, _json.GetAllocator());
	}
}

void Manifest::setVersionFileUrl(std::string versionFileUrl)
{
	if (_json.HasMember(KEY_VERSION_URL) && _json[KEY_VERSION_URL].IsString())
	{
		_json[KEY_VERSION_URL].SetString(versionFileUrl.c_str(), _json.GetAllocator());
	}
	else{
		rapidjson::Value jsonPackageUrl(rapidjson::kStringType);
		jsonPackageUrl.SetString(versionFileUrl.c_str(), _json.GetAllocator());
		_json.AddMember(KEY_VERSION_URL, jsonPackageUrl, _json.GetAllocator());
	}
}