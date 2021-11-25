#include "aaabbb_touch.h"

static struct aaabbb_touch_pdata *touch_pdata;

static int aaabbb_touch_dev_open(struct inode *inode, struct file *file)
{
	struct aaabbb_touch *dev = NULL;
	int i = MINOR(inode->i_rdev);
	struct aaabbb_touch_pdata *touch_pdata;

	pr_info("%s\n", __func__);
	dev = aaabbb_touch_dev_get(i);
	if (!dev) {
		pr_err("%s cant get dev\n", __func__);
		return -ENOMEM;
	}
	touch_pdata = dev_get_drvdata(dev->dev);

	file->private_data = touch_pdata;
	return 0;
}

static ssize_t aaabbb_touch_dev_read(struct file *file, char __user *buf,
			   size_t count, loff_t *pos)
{
	return 0;
}

static ssize_t aaabbb_touch_dev_write(struct file *file,
		const char __user *buf, size_t count, loff_t *pos)
{
	return 0;
}

static unsigned int aaabbb_touch_dev_poll(struct file *file,
		poll_table *wait)
{
	return 0;
}

static long aaabbb_touch_dev_ioctl(struct file *file, unsigned int cmd,
		unsigned long arg)
{
	int ret = -EINVAL;
	int buf[VALUE_TYPE_SIZE] = {0,};
	struct aaabbb_touch_pdata *pdata = file->private_data;
	void __user *argp = (void __user *) arg;
	struct aaabbb_touch_interface *touch_data = pdata->touch_data;
	struct aaabbb_touch *dev = pdata->device;
	int user_cmd = _IOC_NR(cmd);

	if (!pdata || !touch_data || !dev) {
		pr_err("%s invalid memory\n", __func__);
		return -ENOMEM;
	}

	mutex_lock(&dev->mutex);
	ret = copy_from_user(&buf, (int __user *)argp, sizeof(buf));

	pr_info("%s cmd:%d, mode:%d, value:%d\n", __func__, user_cmd, buf[0], buf[1]);

	switch (user_cmd) {
	case SET_CUR_VALUE:
		if (touch_data->setModeValue)
			buf[0] = touch_data->setModeValue(buf[0], buf[1]);
		break;
	case GET_CUR_VALUE:
	case GET_DEF_VALUE:
	case GET_MIN_VALUE:
	case GET_MAX_VALUE:
		if (touch_data->getModeValue)
			buf[0] = touch_data->getModeValue(buf[0], user_cmd);
		break;
	case RESET_MODE:
		if (touch_data->resetMode)
			buf[0] = touch_data->resetMode(buf[0]);
		break;
	case GET_MODE_VALUE:
		if (touch_data->getModeValue)
			ret = touch_data->getModeAll(buf[0], buf);
		break;
	default:
		pr_err("%s don't support mode\n", __func__);
		ret = -EINVAL;
		break;
	}

	if (ret >= 0)
		ret = copy_to_user((int __user *)argp, &buf, sizeof(buf));
	else
		pr_err("%s can't get data from touch driver\n", __func__);

	mutex_unlock(&dev->mutex);

	return ret;
}

static int aaabbb_touch_dev_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations aaabbbtouch_dev_fops = {
	.owner = THIS_MODULE,
	.open = aaabbb_touch_dev_open,
	.read = aaabbb_touch_dev_read,
	.write = aaabbb_touch_dev_write,
	.poll = aaabbb_touch_dev_poll,
	.unlocked_ioctl = aaabbb_touch_dev_ioctl,
	.compat_ioctl = aaabbb_touch_dev_ioctl,
	.release = aaabbb_touch_dev_release,
	.llseek	= no_llseek,
};

static struct aaabbb_touch aaabbb_touch_dev = {
	.misc_dev = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "aaabbb-touch",
		.fops = &aaabbbtouch_dev_fops,
		.parent = NULL,
	},
	.mutex = __MUTEX_INITIALIZER(aaabbb_touch_dev.mutex),
	.palm_mutex = __MUTEX_INITIALIZER(aaabbb_touch_dev.palm_mutex),
	.psensor_mutex = __MUTEX_INITIALIZER(aaabbb_touch_dev.psensor_mutex),
	.wait_queue = __WAIT_QUEUE_HEAD_INITIALIZER(aaabbb_touch_dev.wait_queue),
};

struct aaabbb_touch *aaabbb_touch_dev_get(int minor)
{
	if (aaabbb_touch_dev.misc_dev.minor == minor)
		return &aaabbb_touch_dev;
	else
		return NULL;
}

struct class *get_aaabbb_touch_class()
{
	return aaabbb_touch_dev.class;
}

struct device *get_aaabbb_touch_dev()
{
	return aaabbb_touch_dev.dev;
}

int aaabbbtouch_register_modedata(struct aaabbb_touch_interface *data)
{
	int ret = 0;
	struct aaabbb_touch_interface *touch_data = NULL;

	if (!touch_pdata)
		ret = -ENOMEM;

	touch_data = touch_pdata->touch_data;
	pr_info("%s\n", __func__);

	mutex_lock(&aaabbb_touch_dev.mutex);

	touch_data->setModeValue = data->setModeValue;
	touch_data->getModeValue = data->getModeValue;
	touch_data->resetMode = data->resetMode;
	touch_data->getModeAll = data->getModeAll;
	touch_data->palm_sensor_read = data->palm_sensor_read;
	touch_data->palm_sensor_write = data->palm_sensor_write;
	//touch_data->p_sensor_read = data->p_sensor_read;
	//touch_data->p_sensor_write = data->p_sensor_write;
	//touch_data->panel_vendor_read = data->panel_vendor_read;
	//touch_data->panel_color_read = data->panel_color_read;
	//touch_data->panel_display_read = data->panel_display_read;
	//touch_data->touch_vendor_read = data->touch_vendor_read;

	mutex_unlock(&aaabbb_touch_dev.mutex);

	return ret;
}

int update_palm_sensor_value(int value)
{
	struct aaabbb_touch *dev = NULL;

	mutex_lock(&aaabbb_touch_dev.palm_mutex);

	if (!touch_pdata) {
		mutex_unlock(&aaabbb_touch_dev.palm_mutex);
		return -ENODEV;
	}

	dev = touch_pdata->device;

	if (value != touch_pdata->palm_value) {
		pr_info("%s value:%d\n", __func__, value);
		touch_pdata->palm_value = value;
		touch_pdata->palm_changed = true;
		sysfs_notify(&aaabbb_touch_dev.dev->kobj, NULL,
		     "palm_sensor");
	}

	mutex_unlock(&aaabbb_touch_dev.palm_mutex);
	return 0;
}

static ssize_t palm_sensor_show(struct device *dev,
struct device_attribute *attr, char *buf)
{
	struct aaabbb_touch_pdata *pdata = dev_get_drvdata(dev);

	pdata->palm_changed = false;

	return snprintf(buf, PAGE_SIZE, "%d\n", pdata->palm_value);
}

static ssize_t palm_sensor_store(struct device *dev,
struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	struct aaabbb_touch_pdata *pdata = dev_get_drvdata(dev);

	if (sscanf(buf, "%d", &input) < 0)
			return -EINVAL;

	if (pdata->touch_data->palm_sensor_write)
		pdata->touch_data->palm_sensor_write(!!input);
	else {
		pr_err("%s has not implement\n", __func__);
	}
	pr_info("%s value:%d\n", __func__, !!input);

	return count;
}


static DEVICE_ATTR(palm_sensor, (0664),
		   palm_sensor_show, palm_sensor_store);


static struct attribute *touch_attr_group[] = {
	&dev_attr_palm_sensor.attr,

	NULL,
};

static const struct of_device_id aaabbb_touch_of_match[] = {
	{ .compatible = "aaabbb-touch", },
	{ },
};

static int aaabbb_touch_parse_dt(struct device *dev, struct aaabbb_touch_pdata *data)
{
	int ret;
	struct device_node *np;

	np = dev->of_node;
	if (!np)
		return -ENODEV;

	ret = of_property_read_string(np, "touch,name", &data->name);
	if (ret)
		return ret;

	pr_info("%s touch,name:%s\n", __func__, data->name);

	return 0;
}

static int aaabbb_touch_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct device *dev = &pdev->dev;
	struct aaabbb_touch_pdata *pdata;

	pdata = devm_kzalloc(dev, sizeof(struct aaabbb_touch_pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	pr_info("%s enter\n", __func__);

	ret = aaabbb_touch_parse_dt(dev, pdata);
	if (ret < 0) {
		pr_err("%s parse dt error:%d\n", __func__, ret);
		goto parse_dt_err;
	}

	ret = misc_register(&aaabbb_touch_dev.misc_dev);
	if (ret) {
		pr_err("%s create misc device err:%d\n", __func__, ret);
		goto parse_dt_err;
	}

	if (!aaabbb_touch_dev.class)
		aaabbb_touch_dev.class = class_create(THIS_MODULE, "touch");

	if (!aaabbb_touch_dev.class) {
		pr_err("%s create device class err\n", __func__);
		goto class_create_err;
	}

	aaabbb_touch_dev.dev = device_create(aaabbb_touch_dev.class, NULL, 'T', NULL, "touch_dev");
	if (!aaabbb_touch_dev.dev) {
		pr_err("%s create device dev err\n", __func__);
		goto device_create_err;
	}

	pdata->touch_data = (struct aaabbb_touch_interface *)kzalloc(sizeof(struct aaabbb_touch_interface), GFP_KERNEL);
	if (pdata->touch_data == NULL) {
		ret = -ENOMEM;
		pr_err("%s alloc mem for touch_data\n", __func__);
		goto data_mem_err;
	}

	pdata->device = &aaabbb_touch_dev;
	dev_set_drvdata(aaabbb_touch_dev.dev, pdata);

	touch_pdata = pdata;

	aaabbb_touch_dev.attrs.attrs = touch_attr_group;
	ret = sysfs_create_group(&aaabbb_touch_dev.dev->kobj, &aaabbb_touch_dev.attrs);
	if (ret) {
		pr_err("%s ERROR: Cannot create sysfs structure!:%d\n", __func__, ret);
		ret = -ENODEV;
		goto sys_group_err;
	}

	pr_info("%s over\n", __func__);

	return ret;

sys_group_err:
	kfree(pdata->touch_data);
	pdata->touch_data = NULL;
data_mem_err:
	device_destroy(aaabbb_touch_dev.class, 0);
device_create_err:
	class_destroy(aaabbb_touch_dev.class);
class_create_err:
	misc_deregister(&aaabbb_touch_dev.misc_dev);
parse_dt_err:
	pr_err("%s fail!\n", __func__);
	return ret;
}

static int aaabbb_touch_remove(struct platform_device *pdev)
{
	device_destroy(aaabbb_touch_dev.class, 0);
	class_destroy(aaabbb_touch_dev.class);
	misc_deregister(&aaabbb_touch_dev.misc_dev);
	kfree(touch_pdata->touch_data);
	touch_pdata->touch_data = NULL;
	return 0;
}

static struct platform_driver aaabbb_touch_device_driver = {
	.probe		= aaabbb_touch_probe,
	.remove		= aaabbb_touch_remove,
	.driver		= {
		.name	= "aaabbb-touch",
		.of_match_table = of_match_ptr(aaabbb_touch_of_match),
	}
};

static int __init aaabbb_touch_init(void)
{
	return platform_driver_register(&aaabbb_touch_device_driver);

}

static void __exit aaabbb_touch_exit(void)
{
	platform_driver_unregister(&aaabbb_touch_device_driver);
}

subsys_initcall(aaabbb_touch_init);
module_exit(aaabbb_touch_exit);