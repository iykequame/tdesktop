/*
This file is part of Telegram Desktop,
the official desktop version of Telegram messaging app, see https://telegram.org

Telegram Desktop is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

In addition, as a special exception, the copyright holders give permission
to link the code of portions of this program with the OpenSSL library.

Full license: https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE
Copyright (c) 2014-2017 John Preston, https://desktop.telegram.org
*/
#pragma once

#include "storage/storage_user_photos.h"
#include "base/weak_unique_ptr.h"

class UserPhotosViewer;
class UserPhotosSlice {
public:
	using Key = Storage::UserPhotosKey;

	UserPhotosSlice(Key key);
	UserPhotosSlice(Key key, base::optional<int> fullCount);

	const Key &key() const { return _key; }

	base::optional<int> fullCount() const { return _fullCount; }
	base::optional<int> skippedBefore() const { return _skippedBefore; }
	int skippedAfter() const { return _skippedAfter; }
	base::optional<int> indexOf(PhotoId msgId) const;
	int size() const { return _ids.size(); }
	PhotoId operator[](int index) const;
	base::optional<int> distance(const Key &a, const Key &b) const;

	QString debug() const;

private:
	Key _key;
	std::deque<PhotoId> _ids;
	base::optional<int> _fullCount;
	base::optional<int> _skippedBefore;
	int _skippedAfter = 0;

	friend class UserPhotosViewer;

};

class UserPhotosViewer :
	private base::Subscriber,
	public base::enable_weak_from_this {
public:
	using Key = Storage::UserPhotosKey;

	UserPhotosViewer(Key key, int limitBefore, int limitAfter);

	void start();

	base::Observable<UserPhotosSlice> updated;

private:
	using InitialResult = Storage::UserPhotosResult;
	using SliceUpdate = Storage::UserPhotosSliceUpdate;

	void loadInitial();
	void requestPhotos();
	void applyStoredResult(InitialResult &&result);
	void applyUpdate(const SliceUpdate &update);
	void sliceToLimits();

	void mergeSliceData(
		base::optional<int> count,
		const std::deque<PhotoId> &photoIds,
		base::optional<int> skippedBefore,
		int skippedAfter);

	Key _key;
	int _limitBefore = 0;
	int _limitAfter = 0;
	UserPhotosSlice _data;

};